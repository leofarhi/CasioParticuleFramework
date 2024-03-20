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
        self.OnUpdateAssetsDir()

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

        self.TabAssets.path_asset = MyEntry(self.TabAssets, self.NAME+" Asset Directory")
        self.TabAssets.path_asset.set("Assets/assets-")
        self.TabAssets.path_asset.pack(side=TOP, fill=X)
        self.TabAssets.path_asset.entry.bind("<Return>", self.OnUpdateAssetsDir)
        self.SaveLoadSystem.Add("Assets.Path", self.TabAssets.path_asset)

        self.TabAssets.tabs = ttk.Notebook(self.TabAssets)
        self.TabAssets.tabs.pack(side=TOP, fill=BOTH, expand=1)
        self.TabAssets.tabs.sections = {}

        self.AddAssetsSubtab("Textures", filetypes=[".png",".jpg",".jpeg",".bmp"], default_path="Images")

    def AddAssetsSubtab(self, name, filetypes=None, default_path=""):
        tab = self.AddTab(name, self.TabAssets.tabs)
        tab.DirectoryFile = MyDirectoryFile(tab, filetypes=filetypes)
        tab.DirectoryFile.path.set(default_path)
        tab.DirectoryFile.pack(side=TOP, fill=BOTH, expand=1)
        self.SaveLoadSystem.Add("Assets."+name, tab.DirectoryFile)
        self.TabAssets.tabs.sections[name] = tab

    def OnUpdateAssetsDir(self, *args, **kwargs):
        path = base_path
        if self.TabAssets.path_asset.get() != "":
            path = JoinPath(path,self.TabAssets.path_asset.get())
        for key in self.TabAssets.tabs.sections:
            self.TabAssets.tabs.sections[key].DirectoryFile.SetRootPath(path)

    def GetAllAssets(self):
        self.OnUpdateAssetsDir()
        assets = {}
        for key in self.TabAssets.tabs.sections:
            assets[key] = self.SaveLoadSystem.Get("Assets."+key).GetFiles()
        return assets

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
    
    def OnMakeAssets(self):
        self.TabLibs.Libs.OnEventCallback("OnMakeAssets")
        bin_path = JoinPath(base_path, "Build", "bin")
        asset_path = JoinPath(base_path, self.SaveLoadSystem.Get("Assets.Path").get())
        image_path = JoinPath(asset_path, self.TabAssets.tabs.sections["Textures"].DirectoryFile.path.get())
        AllAssets = self.GetAllAssets()
        images = AllAssets["Textures"]
        CreateDir(JoinPath(bin_path, "assets"))
        bin_img_path = JoinPath(bin_path, "assets", "Images")
        CreateDir(bin_img_path)
        for file in images:
            desti = normalize_path(JoinPath(bin_img_path, os.path.relpath(file, image_path)))
            if IsSpriteFolder(file):
                CreateDir(os.path.dirname(desti))
                if os.path.exists(desti):
                    shutil.rmtree(desti)
                shutil.copytree(file, desti)
            else:
                CreateDir(os.path.dirname(desti))
                shutil.copyfile(file, desti)
        return AllAssets
    
    def CopyAssetsInBuild(self, AllAssets, section_name):
        bin_path = JoinPath(base_path, "Build", "bin")
        asset_path = JoinPath(base_path, self.SaveLoadSystem.Get("Assets.Path").get())
        section_path = JoinPath(asset_path, self.TabAssets.tabs.sections[section_name].DirectoryFile.path.get())
        section = AllAssets[section_name]
        bin_section_path = JoinPath(bin_path, "assets", section_name)
        CreateDir(bin_section_path)
        for file in section:
            desti = normalize_path(JoinPath(bin_section_path, os.path.relpath(file, section_path)))
            CreateDir(os.path.dirname(desti))
            shutil.copyfile(file, desti)

    def OnMake(self):
        self.TabLibs.Libs.OnEventCallback("OnMake")
        self.Makefile.TITLE = self.SaveLoadSystem.Get("General.AppName").get()
        self.Makefile.DEBUG = bool(self.SaveLoadSystem.Get("General.DebugVersion").get())
        clone = self.Makefile.Clone()
        clone.libs += self.GetIncludesDir()
        clone.sources += self.GetIncludesDir()
        clone.defines.append("PROJECT_NAME=\\\""+str(clone.TITLE)+"\\\"")
        clone = self.OnCloneMakefile(clone)
        self.TabLibs.Libs.OnEventCallback("OnCloneMakefile", clone)
        clone.Write()
        clone.MakeFolders()
        self.TabLibs.Libs.OnEventCallback("OnBuild")

    def OnClean(self):
        self.TabLibs.Libs.OnEventCallback("OnClean")

    def OnRun(self):
        self.TabLibs.Libs.OnEventCallback("OnRun")

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
        self.OnMakeAssets()
        self.TabLibs.Libs.OnEventCallback("OnMakeAssetsAfter")
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
        self.OnUpdateAssetsDir()
    
    def AfterLoad(self):
        pass