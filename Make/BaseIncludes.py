from Essentials import *

class MyIncludesFrameNode(LabelFrame):
    def __init__(self, root, distribution, includes, callback = None) -> None:
        LabelFrame.__init__(self, root)
        self.root = root
        self.distribution = distribution
        self.includes = includes
        self.callback = callback
        self.check = MyCheckbutton(self, text=self.includes.showedName, command=self._callback)
        self.check.pack(side=TOP, fill=X)
        self.check.bind("<<ComboboxSelected>>", self._callback)
        self.SaveLoadSystem = SaveLoadSystem()
        self.SaveLoadSystem.Add("Check", self.check)

    def GetIncludesDir(self):
        return self.includes.GetIncludesDir(self, self.distribution)
    
    def OnBuild(self):
        self.includes.OnBuild(self, self.distribution)

    def IsChecked(self):
        return bool(self.check.get())
    
    def _callback(self):
        if self.callback is not None:
            self.callback(self)

    def Save(self):
        return self.SaveLoadSystem.Save()
    
    def Load(self, data):
        if data == None:
            return
        self.SaveLoadSystem.Load(data)
    
class MyIncludesFrame(MyScrollFrame):
    def __init__(self, root, distribution) -> None:
        MyScrollFrame.__init__(self, root)
        self.root = root
        self.distribution = distribution
        self.Frames = []

    def Update(self):
        for frame in self.Frames:
            frame.pack_forget()
        self.Frames = []
        for lib in list(self.distribution.Make.Libs.values()):
            libFrame = lib.Frame(self.frame, self.distribution, callback=self._changed)
            libFrame.pack(side=TOP, fill=X, expand=1, padx=1, pady=1)
            self.Frames.append(libFrame)
        MyScrollFrame.Update(self)

    def _changed(self, frame):
        print("lib", frame.includes.id, "changed")

    def GetIncludesDir(self):
        includes = []
        for libframe in self.Frames:
            if libframe.IsChecked():
                includes += libframe.GetIncludesDir()
        return includes
    
    def OnBuild(self):
        for libframe in self.Frames:
            if libframe.IsChecked():
                libframe.OnBuild()
    
    
    def Save(self):
        data = {}
        for frame in self.Frames:
            data[frame.includes.id] = frame.Save()
        return data

    def Load(self, data):
        if data == None:
            return
        self.Update()
        for frame in self.Frames:
            if frame.includes.id in data:
                frame.Load(data[frame.includes.id])

class BaseIncludes:
    def __init__(self,_file_) -> None:
        self.current_directory = os.path.dirname(os.path.abspath(_file_))
        self.id = normalize_path(os.path.relpath(self.current_directory, base_path))
        self.showedName = self.id
        print("Lib detected:",self.id)

    def Frame(self, root, distribution,*args, **kwargs):
        return MyIncludesFrameNode(root, distribution, self, *args, **kwargs)
    
    def OnBuild(self, node, distribution):
        pass

    def GetDependencies(self):
        return []
    
    def GetIncludesDir(self, distribution):
        return [self.current_directory]