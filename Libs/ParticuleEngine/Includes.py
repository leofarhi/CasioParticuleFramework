from BaseIncludes import *

class Includes(BaseIncludes):
    #ParticuleEngine lib
    def __init__(self) -> None:
        BaseIncludes.__init__(self,__file__)
        self.dirs = {
            "Engine": "engine",
            "Essential": "essential",
        }
        self.distrib_dirs = {
            "Windows": "WindowsLinux",
            "Linux": "WindowsLinux",
        }

    def Frame(self, root, distribution,*args, **kwargs):
        frame = MyIncludesFrameNode(root, distribution, self, *args, **kwargs)
        frame.Selection = LabelFrame(frame, text="Selection")
        frame.Selection.pack(side=TOP, fill=X, expand=1, padx=8, pady=1)
        frame.Selection.lst = []
        for sublib in list(self.dirs.keys()):
            chk = MyCheckbutton(frame.Selection, text=sublib)
            chk.value = self.dirs[sublib]
            chk.pack()
            frame.Selection.lst.append(chk)
            frame.SaveLoadSystem.Add(f"Selection.{dir}", chk)
        return frame

    def GetIncludesDir(self, node, distribution):
        lst = []
        #add all directories in current directory
        for chk in node.Selection.lst:
            if chk.get():
                dist_dir = self.distrib_dirs.get(distribution.NAME)
                if dist_dir:
                    lst.append(JoinPath(self.current_directory,dist_dir,chk.value))
                lst.append(JoinPath(self.current_directory,"All","include",chk.value))
                lst.append(JoinPath(self.current_directory,"All","src",chk.value))
        return lst