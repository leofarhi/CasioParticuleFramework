from Essentials import *
from BaseDistribution import *
import uuid

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
        self.Makefile.sources.append(JoinPath(base_path,"Build","build","extra"))
        self.Makefile.sources.append(JoinPath(base_path,"Sources"))
        self.Makefile.sources.append(JoinPath(base_path,"Libs/DistributionCases/CASIO"))
        self.Makefile.defines.append(CasioCgDistribution.DEFINE)
        self.Makefile.defines.append("OVERWRITE_INIT_ASSET")

    def OnAddGeneralTab(self):
        BaseDistribution.OnAddGeneralTab(self)
        self.TabGeneral.AssetFolderName = MyEntry(self.TabGeneral, "Asset Folder Name")
        self.TabGeneral.AssetFolderName.set("DATA")
        self.TabGeneral.AssetFolderName.grid(row=4, column=0, sticky="nsew")
        self.SaveLoadSystem.Add("General.AssetFolderName", self.TabGeneral.AssetFolderName)
        self.TabGeneral.grid_rowconfigure(4, weight=0)
        self.TabGeneral.grid_rowconfigure(5, weight=1)
        self.TabGeneral.grid_columnconfigure(0, weight=1)

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

        self.TabAssets.path_asset = MyEntry(self.TabAssets, self.NAME+" Asset Directory")
        self.TabAssets.path_asset.set("Assets/assets-cg")
        self.TabAssets.path_asset.pack(side=TOP, fill=X)
        self.TabAssets.path_asset.entry.bind("<Return>", self.OnUpdateAssetsDir)
        self.SaveLoadSystem.Add("Assets.Path", self.TabAssets.path_asset)

        self.TabAssets.tabs = ttk.Notebook(self.TabAssets)
        self.TabAssets.tabs.pack(side=TOP, fill=BOTH, expand=1)
        self.TabAssets.tabs.sections = {}
        self.TabAssets.tabs.sections["Textures"] = self.AddTab("Textures", self.TabAssets.tabs)
        values = ["rgb565_bopti-image","rgb565a_bopti-image","p8_rgb565_bopti-image","p8_rgb565a_bopti-image","p4_rgb565_bopti-image","p4_rgb565a_bopti-image"]
        values += ["External_bopti-image_rgb565a","External_bopti-image_p8_rgb565a","External_bopti-image_p4_rgb565a"]
        #values += ["Font","External_Sprite"]
        self.TabAssets.tabs.sections["Textures"].values = values
        self.TabAssets.tabs.sections["Textures"].DirectoryFile = MyDirectoryFile(self.TabAssets.tabs.sections["Textures"],
                                                                     filetypes=[".png",".jpg",".jpeg",".bmp"],
                                                                     directoryFileType = DirectoryFileCombobox,
                                                                     directoryFileArgs = {"values":values})
        self.TabAssets.tabs.sections["Textures"].DirectoryFile.path.set("Images")
        self.TabAssets.tabs.sections["Textures"].DirectoryFile.pack(side=TOP, fill=BOTH, expand=1)
        self.SaveLoadSystem.Add("Assets.Textures", self.TabAssets.tabs.sections["Textures"].DirectoryFile)

        self.OnUpdateAssetsDir()

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

    def OnMakeAssets(self):
        self.TabLibs.Libs.OnEventCallback("OnMakeAssets")
        if  self.TabGeneral.AssetFolderName.get() == "":
            raise Exception("No Asset Folder Name")
        asset_path_bin = JoinPath(base_path,"Build","bin",self.TabGeneral.AssetFolderName.get())
        asset_path = JoinPath(base_path, self.SaveLoadSystem.Get("Assets.Path").get())
        images = self.SaveLoadSystem.Get("Assets.Textures").GetFiles()
        mkf_path = os.path.dirname(self.Makefile.filename)
        imgs = {}
        img_static = []
        imgDir = set()
        for file, info in images.items():
            info = info["combobox"]
            norm_path_img = normalize_path(os.path.relpath(file, mkf_path))
            imgs[file] = (norm_path_img,info)
            if "External" in info:
                continue
            img_static.append(norm_path_img)
            imgDir.add(os.path.dirname(file))
        for dir in imgDir:
            with open(JoinPath(dir,"fxconv-metadata.txt"), "w") as f:
                f.write("")
        uuids = {}
        CI = self.Make.Plugins["ConvertImageCg"]
        for file,value in imgs.items():
            UUID = str(uuid.uuid4()).replace("-","")
            while UUID in uuids:
                UUID = str(uuid.uuid4()).replace("-","")
            norm_path_img,info = value
            if info not in self.TabAssets.tabs.sections["Textures"].values:
                print("Error: no key for file " + file)
                continue
            uuids[file] = UUID
            if "External" in info:
                CreateDir(asset_path_bin)
                if "External_bopti-image" in info:
                    CI.ConvertImage(file,JoinPath(asset_path_bin,os.path.basename(file))+".bin",info.replace("External_bopti-image_",""))
                elif "External_Sprite" in info:
                    CI.ConvertImageItems(file,JoinPath(asset_path_bin,os.path.basename(file))+".bin.items")
                continue
            fxconv_metadata(JoinPath(os.path.dirname(file),"fxconv-metadata.txt"),file,UUID,info)
        ReplaceLinesInFile(self.Makefile.filename, "IMG_CG", "\n"+"".join([f"    \"{i}\"\n" for i in img_static]))
        CreateDir(JoinPath(base_path,"Build","build","extra"))


        Resources_c = JoinPath(base_path,"Build","build","extra","Resources.c")
        Resources_h = JoinPath(base_path,"Build","build","extra","Resources.h")
        shutil.copy(JoinPath(os.path.dirname(os.path.abspath(__file__)), "templates","Resources.c"), Resources_c)
        shutil.copy(JoinPath(os.path.dirname(os.path.abspath(__file__)), "templates","Resources.h"), Resources_h)
        image_path = JoinPath(asset_path, self.TabAssets.tabs.sections["Textures"].DirectoryFile.path.get())
        bin_img_path = JoinPath("assets", self.TabAssets.tabs.sections["Textures"].DirectoryFile.path.get())

        textImg = "\n"
        for file,ID in uuids.items():
            info = (imgs[file])[1]
            if "External" not in info:
                textImg += "extern bopti_image_t IMG_ASSET_"+str(ID)+";\n"
        textImg += "\n"
        ReplaceLinesInFile(Resources_h, "RSC", textImg)

        textImg = "\n"
        count = 0
        for file,ID in uuids.items():
            desti = normalize_path(JoinPath(bin_img_path, os.path.relpath(file, image_path)))
            info = (imgs[file])[1]
            if "External_bopti-image" in info or "External_Sprite" in info:
                fileBin = normalize_path(JoinPath(self.TabGeneral.AssetFolderName.get(),os.path.basename(file)+".bin"))
                if "External_Sprite" in info:
                    fileBin += ".items"
                textImg += '\t__pc_static_files['+str(count)+'] = __pc_AddTexture((unsigned char*)"'+desti+'", NULL,(char*)"'+fileBin+'");\n'
            else:
                textImg += '\t__pc_static_files['+str(count)+'] = __pc_AddTexture((unsigned char*)"'+desti+'", &IMG_ASSET_'+str(ID)+",NULL);\n"
            count += 1
        textImg += "\n"
        ReplaceLinesInFile(Resources_c, "RSC_LOAD", textImg)
        ReplaceLinesInFile(Resources_c, "RSC", "\nVirtualFile* __pc_static_files["+str(max(1,count))+"];\nint __pc_static_files_count = "+str(count)+";\n")
        self.TabLibs.Libs.OnEventCallback("OnMakeAssetsAfter")

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
        execu = self.SaveLoadSystem.Get("General.AppName").get()+".g3a"
        fromPath = JoinPath(base_path,"Build","bin",execu)
        print("Executable path: " + fromPath)
        toPath = LetterDevice+":\\"+execu
        if not os.path.isfile(fromPath):
            print("No executable found")
            return
        print("Copying executable")
        shutil.copyfile(fromPath, toPath)
        asset_path_bin = JoinPath(base_path,"Build","bin",self.TabGeneral.AssetFolderName.get())
        if os.path.isdir(asset_path_bin):
            print("Copying assets")
            if os.path.isdir(LetterDevice+":\\"+self.TabGeneral.AssetFolderName.get()):
                shutil.rmtree(LetterDevice+":\\"+self.TabGeneral.AssetFolderName.get())
            shutil.copytree(asset_path_bin, LetterDevice+":\\"+self.TabGeneral.AssetFolderName.get())
            print("Assets copied")
        print("Executable copied")
        #print le poid du fichier executable en Ko
        print("Executable size: " + str(os.path.getsize(fromPath)//1024) + " Ko")
        if os.path.isdir(asset_path_bin):
            size = 0
            for path, dirs, files in os.walk(asset_path_bin):
                for f in files:
                    fp = os.path.join(path, f)
                    size += os.path.getsize(fp)
            print("Assets size: " + str(size//1024) + " Ko")
        print("Transfer done")
        


def fxconv_metadata(file_metadata,file_image,UUID,typeImage):
    with open(file_metadata, "a") as f:
        txt = os.path.basename(file_image) + ":\n"
        if typeImage == "Font":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: font\n"
            txt +="  charset: print\n"
            txt +="  grid.size: 13x16\n"
            raise Exception("A terminer!")  
        elif typeImage == "bopti-image":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: bopti-image\n"

        elif typeImage == "rgb565_bopti-image":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: bopti-image\n"
            txt +="  profile: rgb565\n"

        elif typeImage == "rgb565a_bopti-image":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: bopti-image\n"
            txt +="  profile: rgb565a\n"

        elif typeImage == "p8_rgb565_bopti-image":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: bopti-image\n"
            txt +="  profile: p8\n"

        elif typeImage == "p8_rgb565a_bopti-image":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: bopti-image\n"
            txt +="  profile: p8\n"

        elif typeImage == "p4_rgb565_bopti-image":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: bopti-image\n"
            txt +="  profile: p4\n"

        elif typeImage == "p4_rgb565a_bopti-image":
            txt +="  name: IMG_ASSET_"+str(UUID)+"\n"
            txt +="  type: bopti-image\n"
            txt +="  profile: p4\n"
        f.write(txt+"\n")

def Load():
    return CasioCgDistribution