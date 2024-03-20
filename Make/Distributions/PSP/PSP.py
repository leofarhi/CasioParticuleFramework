from Essentials import *
from BaseDistribution import *

class PspDistribution(BaseDistribution):
    NAME = "PSP"
    DEFINE = "DIST_PSP"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        self.Makefile.includesMakefile.append(JoinPath(self.DistributionDir(), "Makefile"))
        self.Makefile.defines.append(PspDistribution.DEFINE)
        self.Makefile.sources.append(JoinPath(base_path,"Libs/DistributionCases/PSP"))
        self.Makefile.libs.append(JoinPath(base_path,"Libs/DistributionCases/PSP"))
        
    def OnAddBottomButton(self):
        self.AddBottomButton("Run", self.Run)
        self.AddBottomButton("Clean", self.Clean)
        self.AddBottomButton("Rebuild", self.Rebuid)

    def OnAddMakeTab(self):
        BaseDistribution.OnAddMakeTab(self)
        self.TabMake.RunCmd.label.config(text="Path of the emulator executable")

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

        self.TabAssets.tabs.sections["Textures"] = self.AddTab("Textures", self.TabAssets.tabs)
        values = ["rgb565a","p8_rgb565a","p4_rgb565a"]
        self.TabAssets.tabs.sections["Textures"].values = values
        self.TabAssets.tabs.sections["Textures"].DirectoryFile = MyDirectoryFile(self.TabAssets.tabs.sections["Textures"],
                                                                     filetypes=[".png",".jpg",".jpeg",".bmp"],
                                                                     directoryFileType = DirectoryFileCombobox,
                                                                     directoryFileArgs = {"values":values})
        self.TabAssets.tabs.sections["Textures"].DirectoryFile.path.set("Images")
        self.TabAssets.tabs.sections["Textures"].DirectoryFile.pack(side=TOP, fill=BOTH, expand=1)
        self.SaveLoadSystem.Add("Assets.Textures", self.TabAssets.tabs.sections["Textures"].DirectoryFile)

        self.AddAssetsSubtab("Fonts", filetypes=[".png",".jpg",".jpeg",".bmp"], default_path="Fonts")

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
        CI = self.Make.Plugins["ConvertImageCg"]
        for file in images:
            desti = normalize_path(JoinPath(bin_img_path, os.path.relpath(file, image_path)))
            info = images[file].get("combobox",None)
            if info == "" or info == None:
                raise Exception(f"Image {file} has no format")
            if IsSpriteFolder(file):
                continue
                CreateDir(os.path.dirname(desti))
                if os.path.exists(desti):
                    shutil.rmtree(desti)
                shutil.copytree(file, desti)
            else:
                CreateDir(os.path.dirname(desti))
                #shutil.copyfile(file, desti)
                CI.ConvertImage(file, desti+".bin",info, endian="<")
        image_path = JoinPath(asset_path, self.TabAssets.tabs.sections["Fonts"].DirectoryFile.path.get())
        images = AllAssets["Fonts"]
        bin_img_path = JoinPath(bin_path, "assets", "Fonts")
        CreateDir(bin_img_path)
        for file in images:
            desti = normalize_path(JoinPath(bin_img_path, os.path.relpath(file, image_path)))
            if info == "" or info == None:
                raise Exception(f"Image {file} has no format")
            CreateDir(os.path.dirname(desti))
            info = {"char_width":8,"char_height":8,"column":16,"row":16}
            CI.ConvertSpecialFont(file, desti+".bin",info, endian="<")
        return AllAssets

    def Run(self):
        print("Running")
        self.OnRun()
        path = self.ReplaceVars(self.SaveLoadSystem.Get("Make.RunCmd").get())
        dir_exe = os.path.dirname(path)
        exe = os.path.basename(path)
        bin_path = JoinPath(base_path,"Build","bin","EBOOT.PBP")
        cmdString = f"{exe} {bin_path}"
        subprocess.call(cmdString, shell=True, cwd=dir_exe)

    def Rebuid(self):
        self.Clean()
        self.Build()

def Load():
    return PspDistribution