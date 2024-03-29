from Essentials import *
from BaseDistribution import *
from BaseIncludes import *

# get the current directory
current_directory = os.path.dirname(os.path.abspath(__file__))
distribution_files = []
for dir in os.listdir(JoinPath(current_directory,"Distributions")):
    #check if is directory
    if os.path.isdir(JoinPath(current_directory,"Distributions",dir)):
        file = JoinPath(current_directory,"Distributions",dir,dir+".py")
        if os.path.exists(file):
            distribution_files.append(file)
distributions_modules = []
# import all the classes dynamically
for file in distribution_files:
    module = os.path.basename(file).replace(".py","")
    module = importlib.import_module(f"Distributions.{module}.{module}")
    distributions_modules.append(module)

distributions_class = []
distributions_names = []
for module in distributions_modules:
    try:
        dist = module.Load()
        #check if NAME is defined
        if not hasattr(dist, "NAME"):
            raise Exception(f"Class {dist.__name__} doesn't have NAME defined")
        if not issubclass(dist, BaseDistribution):
            raise Exception(f"Class {dist.__name__} is not a subclass of BaseDistribution")
        if dist.NAME in distributions_names:
            raise Exception(f"Duplicate distribution name: {dist.NAME}")
        distributions_class.append(dist)
        distributions_names.append(dist.NAME)
    except Exception as e:
        print(f"Error loading {module.__name__}: {e}")

if len(distributions_class) == 0:
    raise Exception("No distributions loaded")

Libs_modules = []
for dir in os.listdir(JoinPath(base_path,"Libs")):
    #check if is directory
    if os.path.isdir(JoinPath(base_path,"Libs",dir)):
        file = JoinPath(base_path,"Libs",dir,"Includes.py")
        if os.path.exists(file):
            #import by path
            spec = importlib.util.spec_from_file_location(f"..Libs.{dir}.Includes", file)
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
            Libs_modules.append(module)

Libs = {}
for module in Libs_modules:
    #check if module.Includes() is defined
    if not hasattr(module, "Includes"):
        raise Exception(f"Module {module.__name__} doesn't have Includes defined")
    try:
        instance = module.Includes()
        Libs[instance.id] = instance
    except Exception as e:
        print(f"Error loading {module.__name__}: {e}")

Plugins = {}
plugins_path = JoinPath(current_directory,"Plugins")
for dir in os.listdir(plugins_path):
    #check if is directory
    if os.path.isdir(JoinPath(plugins_path,dir)):
        file = JoinPath(plugins_path,dir,"Plugin.py")
        if os.path.exists(file):
            #import by path
            spec = importlib.util.spec_from_file_location(f"Plugins.{dir}.Plugin", file)
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
            Plugins[dir] = module

class Make(Tk):
    def __init__(self, *args, **kwargs):
        Tk.__init__(self, *args, **kwargs)
        self.title("Make")
        self.geometry("800x600")
        #self.resizable(0, 0)
        self.protocol("WM_DELETE_WINDOW", self.on_closing)

        self.MenuDistributions = MyCombobox(self, state="readonly")
        self.MenuDistributions.SetValues([d.NAME for d in distributions_class])
        self.MenuDistributions.pack(side=TOP, fill=X)
        self.MenuDistributions.combobox.bind("<<ComboboxSelected>>", self.DistributionSelected)

        self.Libs = Libs
        self.Plugins = Plugins
        self.Distributions = []

        self.SaveLoadSystem = SaveLoadSystem()
        self.SaveLoadSystem.Add("MenuDistributions", self.MenuDistributions)

        self.MainFrame = Frame(self)
        self.MainFrame.pack(side=TOP, fill=BOTH, expand=True)
        for dist in distributions_class:
            self.Distributions.append(dist(self, self.MainFrame))
        self.Distributions[0].pack(side=TOP, fill=BOTH, expand=True)
        self.CurrentDistribution = self.Distributions[0]
        self.Load()

    def DistributionSelected(self, *args, **kwargs):
        self.CurrentDistribution.ConfigSelector.Save()
        for dist in self.Distributions:
            dist.pack_forget()
        for i, dist in enumerate(distributions_class):
            if self.MenuDistributions.get() == dist.NAME:
                self.Distributions[i].pack(side=TOP, fill=BOTH, expand=True)
                self.Distributions[i].ConfigSelector.Reload()
                self.CurrentDistribution = self.Distributions[i]
                break

    def on_closing(self):
        self.Save()
        if messagebox.askokcancel("Quit", "Do you want to quit?"):
            self.destroy()

    def Save(self):
        all_data = {}
        for dist in self.Distributions:
            all_data[dist.NAME] = dist.ConfigSelector.Save()
        all_data["SaveLoadSystem"] = self.SaveLoadSystem.Save()
        with open(JoinPath(current_directory,"config.json"), "w") as f:
            json.dump(all_data, f, indent=4)
        print("Saved")

    def Load(self):
        if os.path.exists(JoinPath(current_directory,"config.json")):
            with open(JoinPath(current_directory,"config.json"), "r") as f:
                all_data = json.load(f)
                for dist in self.Distributions:
                    if dist.NAME in all_data:
                        dist.ConfigSelector.Load(all_data[dist.NAME])
                self.SaveLoadSystem.Load(all_data.get("SaveLoadSystem",{}))
                self.DistributionSelected()
            for dist in self.Distributions:
                dist.AfterLoad()
            print("Loaded")
            


if __name__ == "__main__":
    app = Make()
    app.mainloop()