from BaseIncludes import *

class Includes(BaseIncludes):
    #ParticuleEngine lib
    def __init__(self) -> None:
        BaseIncludes.__init__(self,__file__)

    def Frame(self, root, distribution,*args, **kwargs):
        frame = MyIncludesFrameNode(root, distribution, self, *args, **kwargs)
        frame.Selection = LabelFrame(frame, text="Selection")
        frame.Selection.pack(side=TOP, fill=X, expand=1, padx=8, pady=1)
        frame.Selection.lst = []
        for dir in os.listdir(self.current_directory):
            if dir.startswith("__"):
                continue
            if os.path.isdir(JoinPath(self.current_directory,dir)):
                chk = MyCheckbutton(frame.Selection, text=dir)
                chk.path = JoinPath(self.current_directory,dir)
                chk.pack()
                frame.Selection.lst.append(chk)
                frame.SaveLoadSystem.Add(f"Selection.{dir}", chk)
        return frame

    def GetIncludesDir(self, node, distribution):
        lst = []
        #add all directories in current directory
        for dir in node.Selection.lst:
            if dir.get():
                lst.append(dir.path)
        return lst