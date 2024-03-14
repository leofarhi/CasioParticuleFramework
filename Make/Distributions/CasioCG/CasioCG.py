from Essentials import *
from BaseDistribution import *

class CasioMakefile(Makefile):
    def __init__(self, filename):
        Makefile.__init__(self, filename)

    def Write(self):
        pass


class CasioCgDistribution(BaseDistribution):
    NAME = "Casio CG"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        self.Makefile = Makefile(JoinPath(os.path.dirname(os.path.abspath(__file__)), "CMakeLists.txt"))

    def OnAddMakeTab(self):
        self.TabMake = self.AddTab("Make")

        self.TabMake.MakeCmd = MyEntry(self.TabMake, "Make Command")
        self.TabMake.MakeCmd.grid(row=2, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("Make.MakeCmd", self.TabMake.MakeCmd)
        #set row and column to center
        self.TabMake.grid_rowconfigure(3, weight=1)
        self.TabMake.grid_columnconfigure(0, weight=1)

    def OnAddAssetsTab(self):
        self.TabAssets = self.AddTab("Assets")
        self.TabAssets.tabs = ttk.Notebook(self.TabAssets)
        self.TabAssets.tabs.pack(side=TOP, fill=BOTH, expand=1)
        self.TabAssets.tabs.Textures = self.AddTab("Textures", self.TabAssets.tabs)
        self.TabAssets.tabs.Textures.DirectoryFile = MyDirectoryFile(self.TabAssets.tabs.Textures,
                                                                     filetypes=[".png",".jpg",".jpeg",".bmp"],
                                                                     directoryFileType = DirectoryFileCombobox,
                                                                     directoryFileArgs = {"values":["rgb565_bopti-image","rgb565a_bopti-image","p8_rgb565_bopti-image","p8_rgb565a_bopti-image","p4_rgb565_bopti-image","p4_rgb565a_bopti-image"]})
        self.TabAssets.tabs.Textures.DirectoryFile.pack(side=TOP, fill=BOTH, expand=1)
        self.SaveLoadSystem.Add("Assets.Textures", self.TabAssets.tabs.Textures.DirectoryFile)

    def OnAddBottomButton(self):
        self.AddBottomButton("Transfer to casio", None)
        self.AddBottomButton("Clean", self.Clean)
        self.AddBottomButton("Build", self.Build)

    def Clean(self):
        print("Cleaning")
        self.OnClean()
        cmdString = "rm -rf " + JoinPath(base_path,"Build","bin")
        print(cmdString)
        subprocess.call(cmdString, shell=True)
        cmdString = "rm -rf " + JoinPath(base_path,"Build","build")
        print(cmdString)
        subprocess.call(cmdString, shell=True)

    def Build(self):
        print("Building")
        self.OnMake()
        cmdString = self.ReplaceVars(self.SaveLoadSystem.Get("Make.MakeCmd").get())
        subprocess.call(cmdString, shell=True, cwd=os.path.dirname(self.Makefile.filename))
        

def Load():
    return CasioCgDistribution