from Essentials import *
from BaseDistribution import *

class CasioMakefile(Makefile):
    def __init__(self, filename):
        Makefile.__init__(self, filename)
        self.ASSETS = JoinPath(self.path_dirname,"Assets","assets-cg")

    def Write(self):
        ReplaceLinesInFile(self.filename, "NAME_APP_FIELD", "\nset(NAME_APP \"" + self.TITLE + "\")\n")
        ReplaceLinesInFile(self.filename, "DIR_OUT_FIELD", "\nset(DIR_OUT \"" + normalize_path(self.OUTPUT) + "\")\n")
        ReplaceLinesInFile(self.filename, "LIBS_DIR_FIELD", "\nset(LIBS_DIR \"" + normalize_path(self.LibsDir) + "\")\n")
        ReplaceLinesInFile(self.filename, "ASSETS_DIR_FIELD", "\nset(ASSETS_DIR \"" + normalize_path(self.ASSETS) + "\")\n")
        search = self.libs+self.sources
        sources = []
        for source in search:
            if os.path.isdir(source):
                sources += GetAllFilesInSubFolders(source, filetypes=[".c",".cpp",".h",".hpp"])
            elif os.path.isfile(source):
                sources.append(source)
        #remove duplicates
        sources = list(set(sources))
        sources = [normalize_path(os.path.relpath(s, os.path.dirname(self.filename))) for s in sources]
        sources = "\n"+"".join([f"    {i}\n" for i in sources])
        ReplaceLinesInFile(self.filename, "SRC", sources)

        defines = self.defines.copy()
        if self.DEBUG:
            defines.append("DEBUG_MODE")
        defines = "\nadd_definitions("+" ".join([f"-D{i}" for i in defines])+ ")\n"
        ReplaceLinesInFile(self.filename, "DEFINE_FIELD", defines)
        libs = [normalize_path(os.path.relpath(i, os.path.dirname(self.filename))) for i in self.libs]
        libs = "\n"+"".join([f"include_directories(\"{i}\")\nlink_directories(\"{i}\")\n" for i in libs])
        ReplaceLinesInFile(self.filename, "LIBS", libs)


class CasioCgDistribution(BaseDistribution):
    NAME = "Casio CG"
    DEFINE = "DIST_CASIO_CG"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        self.Makefile = CasioMakefile(JoinPath(os.path.dirname(os.path.abspath(__file__)), "CMakeLists.txt"))
        self.Makefile.sources.append(JoinPath(base_path,"Sources"))
        self.Makefile.sources.append(JoinPath(base_path,"Libs/DistributionCases/CASIO"))
        self.Makefile.defines.append(CasioCgDistribution.DEFINE)

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
        self.AddBottomButton("Transfer to casio", self.Televerse)
        self.AddBottomButton("Clean", self.Clean)
        self.AddBottomButton("Build", self.Build)

    def UpdateInfos(self):
        BaseDistribution.UpdateInfos(self)
        self.InfosVariables["OUTPUT"] = normalize_path(self.Makefile.OUTPUT)
        self.InfosVariables["BUILD"] = normalize_path(self.Makefile.BUILD)

    def Clean(self):
        print("Cleaning")
        self.OnClean()
        cmdString = "rm -rf " + JoinPath(base_path,"Build","bin")
        print(cmdString)
        subprocess.call(cmdString, shell=True)
        cmdString = "rm -rf " + JoinPath(base_path,"Build","build")
        print(cmdString)
        subprocess.call(cmdString, shell=True)
        cmdString = "make clean"
        subprocess.call(cmdString, shell=True, cwd=JoinPath(base_path,"Make"))

    def Build(self):
        print("Building")
        self.OnMake()
        cmdString = self.ReplaceVars(self.SaveLoadSystem.Get("Make.MakeCmd").get())
        subprocess.call(cmdString, shell=True, cwd=os.path.dirname(self.Makefile.filename))

    def Televerse(self):
        #detect Letter Device
        #check if path exist for each Letter
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        LetterDevice = None
        for letter in alphabet:
            if os.path.isdir(letter+":\\@MainMem"):
                LetterDevice = letter
                break
        if LetterDevice == None:
            print("No Letter Device found")
            return
        print("Letter Device found: " + LetterDevice)
        #copy executable
        execu = self.Makefile.TITLE+".g3a"
        fromPath = JoinPath(base_path,"Build","bin",execu)
        toPath = LetterDevice+":\\"+execu
        if not os.path.isfile(fromPath):
            print("No executable found")
            return
        print("Copying executable")
        shutil.copyfile(fromPath, toPath)
        """#copy assets
        assetFolder = self.AssetNameFolderString.get()
        assetBinPath = JoinPath(base_path,"Build","bin",assetFolder)
        if os.path.isdir(assetBinPath):
            if os.path.isdir(LetterDevice+":\\"+assetFolder):
                shutil.rmtree(LetterDevice+":\\"+assetFolder)
            shutil.copytree(assetBinPath, LetterDevice+":\\"+assetFolder)
            print("Assets copied")
        """
        print("Executable copied")
        #print le poid du fichier executable en Ko
        print("Executable size: " + str(os.path.getsize(fromPath)//1024) + " Ko")
        """#print le poid du fichier assets en Ko
        if os.path.isdir(assetBinPath):
            size = 0
            for path, dirs, files in os.walk(assetBinPath):
                for f in files:
                    fp = os.path.join(path, f)
                    size += os.path.getsize(fp)
            print("Assets size: " + str(size//1024) + " Ko")
        """
        print("Transfer done")
        

def Load():
    return CasioCgDistribution