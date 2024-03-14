from Essentials import *
from BaseMakefile import *
from BaseIncludes import *


class BaseDistribution(Frame):
    NAME = "Base Distribution"
    DEFINE = "DIST_BASE"
    def __init__(self, Make, MainFrame):
        Frame.__init__(self, MainFrame)
        self.Make = Make
        self.MainFrame = MainFrame

        self.Makefile = Makefile(JoinPath(base_path,"Make", "Makefile"))
        self.Makefile.sources.append(JoinPath(base_path,"Sources"))
        
        self.SaveLoadSystem = SaveLoadSystem()
        self.InfosVariables = {}

        self.ConfigSelector = MyConfigSelector(self)
        self.ConfigSelector.pack(side=TOP, fill=X)

        self.tabs = ttk.Notebook(self)
        self.tabs.pack(side=TOP, fill=BOTH, expand=1)

        self.OnAddGeneralTab()
        self.OnAddLibsTab()
        self.OnAddMakeTab()
        self.OnAddBottomButton()
        self.OnAddAssetsTab()

    def ReplaceVars(self, cmd):
        self.UpdateInfos()
        #detect all $(varName) and replace by self.SaveLoadSystem.Get(varName))
        for keys in self.SaveLoadSystem.Keys():
            cmd = cmd.replace(f"$({keys})", str(self.SaveLoadSystem.Get(keys).Save()))
        for key in self.InfosVariables:
            cmd = cmd.replace(f"$({key})", str(self.InfosVariables[key]))
        return cmd
    
    def UpdateInfos(self):
        make_dir = os.path.dirname(self.Makefile.filename)
        self.InfosVariables["OUTPUT"] = os.path.abspath(JoinPath(make_dir, self.Makefile.OUTPUT))
        self.InfosVariables["BUILD"] = os.path.abspath(JoinPath(make_dir, self.Makefile.BUILD))
        self.InfosVariables["LIBS_DIR"] = os.path.abspath(JoinPath(make_dir, self.Makefile.LibsDir))

    def DistributionDir(self):
        return os.path.dirname(os.path.abspath(inspect.getfile(self.__class__)))

    def AddTab(self, name, notebook = None,*args, **kwargs):
        if notebook is None:
            notebook = self.tabs
        frame = Frame(notebook)
        frame.pack(side=TOP, fill=BOTH, expand=1)
        notebook.add(frame, text=name)
        return frame
    
    def AddBottomButton(self, name, command):
        button = MyButton(self, text=name)
        button.AddCommand(self.Make.Save)
        if command is not None:
            button.AddCommand(command)
        button.pack(fill=X, padx=1, pady=1)
        return button
    
    def OnAddAssetsTab(self):
        self.TabAssets = self.AddTab("Assets")
        self.TabAssets.tabs = ttk.Notebook(self.TabAssets)
        self.TabAssets.tabs.pack(side=TOP, fill=BOTH, expand=1)
        self.TabAssets.tabs.Textures = self.AddTab("Textures", self.TabAssets.tabs)
        self.TabAssets.tabs.Textures.DirectoryFile = MyDirectoryFile(self.TabAssets.tabs.Textures, filetypes=[".png",".jpg",".jpeg",".bmp"])
        self.TabAssets.tabs.Textures.DirectoryFile.pack(side=TOP, fill=BOTH, expand=1)
        self.SaveLoadSystem.Add("Assets.Textures", self.TabAssets.tabs.Textures.DirectoryFile)

    def OnAddLibsTab(self):
        self.TabLibs = self.AddTab("Libs")
        self.TabLibs.Libs = MyIncludesFrame(self.TabLibs, self)
        self.TabLibs.Libs.pack(side=TOP, fill=BOTH, expand=1)
        self.TabLibs.Libs.Update()
        self.SaveLoadSystem.Add("Libs", self.TabLibs.Libs)

    def GetIncludesDir(self):
        return self.TabLibs.Libs.GetIncludesDir()
    
    def OnAddBottomButton(self):
        self.AddBottomButton("Run", self.Run)
        self.AddBottomButton("Clean", self.Clean)
        self.AddBottomButton("Build", self.Build)

    def OnCloneMakefile(self, clone):
        return clone

    def OnMake(self):
        clone = self.Makefile.Clone()
        clone.TITLE = self.SaveLoadSystem.Get("General.AppName").get()
        clone.DEBUG = bool(self.SaveLoadSystem.Get("General.DebugVersion").get())
        clone.libs += self.GetIncludesDir()
        clone.sources += self.GetIncludesDir()
        clone = self.OnCloneMakefile(clone)
        clone.Write()
        clone.MakeFolders()
        self.TabLibs.Libs.OnBuild()

    def OnClean(self):
        pass

    def OnRun(self):
        pass

    def Clean(self):
        print("Cleaning")
        self.OnClean()
        cmdString = self.ReplaceVars(self.SaveLoadSystem.Get("Make.CleanCmd").get())
        subprocess.call(cmdString, shell=True, cwd=os.path.dirname(self.Makefile.filename))

    def Run(self):
        print("Running")
        self.OnRun()
        cmdString = self.ReplaceVars(self.SaveLoadSystem.Get("Make.RunCmd").get())
        subprocess.call(cmdString, shell=True, cwd=JoinPath(base_path,"Build","bin"))

    def Build(self):
        print("Building")
        self.OnMake()
        cmdString = self.ReplaceVars(self.SaveLoadSystem.Get("Make.MakeCmd").get())
        subprocess.call(cmdString, shell=True, cwd=os.path.dirname(self.Makefile.filename))
    
    def OnAddGeneralTab(self):
        self.TabGeneral = self.AddTab("General")
        self.TabGeneral.DebugVersion = MyCheckbutton(self.TabGeneral, text="Debug Version")
        self.TabGeneral.DebugVersion.grid(row=0, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("General.DebugVersion", self.TabGeneral.DebugVersion)

        self.TabGeneral.AppName = MyEntry(self.TabGeneral, "App Name")
        self.TabGeneral.AppName.grid(row=1, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("General.AppName", self.TabGeneral.AppName)

        self.TabGeneral.AppVersion = MyEntry(self.TabGeneral, "App Version")
        self.TabGeneral.AppVersion.grid(row=2, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("General.AppVersion", self.TabGeneral.AppVersion)

        self.TabGeneral.AppAuthor = MyEntry(self.TabGeneral, "App Author")
        self.TabGeneral.AppAuthor.grid(row=3, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("General.AppAuthor", self.TabGeneral.AppAuthor)
        #set row and column to center
        self.TabGeneral.grid_rowconfigure(4, weight=1)
        self.TabGeneral.grid_columnconfigure(0, weight=1)

    def OnAddMakeTab(self):
        self.TabMake = self.AddTab("Make")
        self.TabMake.RunCmd = MyEntry(self.TabMake, "Run Command")
        self.TabMake.RunCmd.grid(row=0, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("Make.RunCmd", self.TabMake.RunCmd)

        self.TabMake.CleanCmd = MyEntry(self.TabMake, "Clean Command")
        self.TabMake.CleanCmd.grid(row=1, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("Make.CleanCmd", self.TabMake.CleanCmd)

        self.TabMake.MakeCmd = MyEntry(self.TabMake, "Make Command")
        self.TabMake.MakeCmd.grid(row=2, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("Make.MakeCmd", self.TabMake.MakeCmd)
        #set row and column to center
        self.TabMake.grid_rowconfigure(3, weight=1)
        self.TabMake.grid_columnconfigure(0, weight=1)

    
    def Save(self):
        return self.SaveLoadSystem.Save()

    def Load(self, data):
        if data is None:
            data = {}
        self.SaveLoadSystem.Load(data)
    
    def AfterLoad(self):
        pass