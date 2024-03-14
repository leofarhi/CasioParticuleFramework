import os, sys
from tkinter import *
from tkinter import ttk
from tkinter import messagebox
from tkinter import simpledialog
from tkinter import filedialog
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import Spinbox
from tkinter import Checkbutton
import importlib
import json
import shutil
import uuid
import inspect
#subprocess
import subprocess
from PIL import Image
from functools import partial

def JoinPath(*args):
    return os.path.join(*args)

base_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
build_path = JoinPath(base_path, "Build","build")
bin_path = JoinPath(base_path, "Build","bin")

def CreateDir(path):
    if not os.path.exists(path):
        os.makedirs(path)

def normalize_path(path):
    path = os.path.normpath(path)
    #replace \ by /
    path = path.replace("\\","/")
    return path

class SaveLoadSystem:
    def __init__(self):
        self.data = {}

    def Add(self, name, obj):
        self.data[name] = obj

    def Remove(self, name):
        del self.data[name]

    def Get(self, name):
        return self.data.get(name, None)
    
    def Keys(self):
        return self.data.keys()

    def Save(self):
        data = {}
        for name, obj in self.data.items():
            data[name] = obj.Save()
        return data
    
    def Load(self, data):
        for name, obj in self.data.items():
            try:
                if name in data:
                    obj.Load(data[name])
                else:
                    obj.Load(None)
            except Exception as e:
                print(f"Error loading {name}: {e}")

class AfterLoadSystem:
    def __init__(self, save_function, load_function):
        self.load_function = load_function
        self.save_function = save_function
        self.dataLoaded = None

    def Save(self):
        return self.save_function()
    
    def Load(self, data):
        self.dataLoaded = data
        self.load_function(data)

    def AfterLoad(self):
        if self.dataLoaded is not None:
            self.load_function(self.dataLoaded)

class FindInFileObject:
    def __init__(self, filename = "", readAll = "", content = "", start = 0, end = 0, sep_length = 0):
        self.filename = filename
        self.readAll = readAll
        self.content = content
        self.start = start
        self.end = end
        self.sep_length = sep_length

def FindInFile(filename, varName):
    #find var in file between #/*<VAR>*/ and #/*</VAR>*/
    Text = ""
    with open(filename, "r") as f:
        Text = f.read()
    #find start and end of var
    start = Text.find("#/*<" + varName + ">*/")
    end = Text.find("#/*</" + varName + ">*/")
    LEN = len("#/*<" + varName + ">*/")
    if start == -1 or end == -1:
        start = Text.find("//*<" + varName + ">*/")
        end = Text.find("//*</" + varName + ">*/")
    if start == -1 or end == -1:
        print("Error: could not find var " + varName + " in " + filename)
        return None
    #return object with all data
    return FindInFileObject(filename, Text, Text[start + LEN:end], start + LEN, end, LEN)

def ReplaceLinesInFile(filename, varName, line):
    obj = FindInFile(filename, varName)
    if obj is None:
        return
    #add line to text but dont remove start and end
    Text = obj.readAll[:obj.start] + line + obj.readAll[obj.end:]
    #write text to file
    with open(filename, "w") as f:
        f.write(Text)

def GetLinesInFile(filename, varName):
    obj = FindInFile(filename, varName)
    if obj is None:
        return None
    return obj.content

def AddLinesInFile(filename, varName, line, OnTop = False):
    obj = FindInFile(filename, varName)
    if obj is None:
        return
    if OnTop:
        Text = obj.readAll[:obj.start] + line + obj.content + obj.readAll[obj.end:]
    else:
        Text = obj.readAll[:obj.end] + line + obj.readAll[obj.end:]
    #write text to file
    with open(filename, "w") as f:
        f.write(Text)


def merge_directories(source_dir, destination_dir):
    for item in os.listdir(source_dir):
        s = os.path.join(source_dir, item)
        d = os.path.join(destination_dir, item)
        if os.path.isdir(s):
            if not os.path.exists(d):
                os.makedirs(d)
            merge_directories(s, d)
        else:
            if not os.path.exists(d):
                shutil.copy2(s, d)

def GetAllSubFolders(root):
    queue = [root]
    folders = []
    while len(queue) > 0:
        folder = queue.pop(0)
        folders.append(folder)
        for file in os.listdir(folder):
            if os.path.isdir(os.path.join(folder, file)):
                queue.append(os.path.join(folder, file))
    return folders

def GetAllFilesInSubFolders(srcFolder, filetypes = []):
    listFiles = []
    for root, dirs, files in os.walk(srcFolder):
        for file in files:
            if True in [bool(file.endswith(ext)) for ext in filetypes]:
                listFiles.append(os.path.join(root, file))
    return listFiles

#tkinter zone

class MyCheckbutton(Checkbutton):
    def __init__(self, *args, **kwargs):
        Checkbutton.__init__(self, *args, **kwargs)
        self.var = IntVar()
        self.config(variable=self.var)
    
    def get(self):
        return bool(self.var.get())
    
    def set(self, value):
        self.var.set(int(value))

    def Save(self):
        return self.get()
    
    def Load(self, value):
        if type(value) == bool:
            self.set(value)
        else:
            self.set(False)

class MyEntry(LabelFrame):
    def __init__(self, root, name="", *args, **kwargs):
        LabelFrame.__init__(self, root)
        self.var = StringVar()
        if name != "":
            self.label = Label(self, text=name)
            self.label.pack(side=LEFT, fill=BOTH, padx=5, pady=2)
        self.entry = Entry(self, textvariable=self.var, *args, **kwargs)
        self.entry.pack(side=LEFT, fill=BOTH, expand=True)

    def get(self):
        return self.var.get()
    
    def set(self, value):
        self.var.set(value)

    def Save(self):
        return self.get()
    
    def Load(self, value):
        if type(value) == str:
            self.set(value)
        else:
            self.set("")

class MyCombobox(LabelFrame):
    def __init__(self, root, name="", *args, **kwargs):
        LabelFrame.__init__(self, root)
        self.var = StringVar()
        if name != "":
            self.label = Label(self, text=name)
            self.label.pack(side=LEFT, fill=BOTH, expand=True)
        self.combobox = ttk.Combobox(self, textvariable=self.var, *args, **kwargs)
        self.combobox.pack(side=LEFT, fill=BOTH, expand=True)

    def SetValues(self, values):
        self.combobox['values'] = values
        self.combobox.current(0)

    def get(self):
        return self.var.get()
    
    def set(self, value):
        self.var.set(value)

    def Save(self):
        return self.get()
    
    def Load(self, value):
        if type(value) == str:
            if value in self.combobox['values']:
                self.set(value)
            else:
                self.set("")
        else:
            self.set("")

class MyConfigSelector(LabelFrame):
    def __init__(self, distributionObject, *args, **kwargs):
        LabelFrame.__init__(self, distributionObject, text="Config Selector", *args, **kwargs)
        self.distributionObject = distributionObject
        self.var = StringVar()
        self.selector = ttk.Combobox(self, state="readonly")
        self.selector.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)
        self.selector.bind("<<ComboboxSelected>>", self.SelectConfig)
        self.button = Button(self, text="New", command=self.NewConfig)
        self.button.grid(row=0, column=1, padx=5, pady=5)
        self.button = Button(self, text="Delete", command=self.DeleteConfig)
        self.button.grid(row=0, column=2, padx=5, pady=5)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)
        self.grid_columnconfigure(2, weight=0)
        self.grid_columnconfigure(3, weight=0)
        self.grid_columnconfigure(4, weight=0)
        self.grid_rowconfigure(0, weight=0)

        self.Datas = {"default": dict()}
        self.CurrentSet = "default"
        self.selector['values'] = list(self.Datas.keys())
        self.selector_set("default")

    def current(self, index):
        if index == "" or index is None:
            self.CurrentSet = None
        self.selector.current(index)
        self.CurrentSet = self.selector.get()

    def selector_set(self, value):
        self.selector.set(value)
        self.CurrentSet = value

    def update(self):
        self.selector['values'] = list(self.Datas.keys())
        if len(self.Datas) > 0:
            self.current(0)
        else:
            self.selector_set("")
            

    def SelectConfig(self, event):
        if self.CurrentSet is not None:
            self.Save(self.CurrentSet)
        self.distributionObject.Load(self.Datas[self.selector.get()])
        self.CurrentSet = self.selector.get()

    def NewConfig(self):
        if self.CurrentSet is not None:
            self.Save(self.CurrentSet)
        name = simpledialog.askstring("New Config", "Enter the name of the new config")
        if name is None or name == "":
            return
        if name in self.Datas:
            messagebox.showerror("Error", "Config already exists")
            return
        self.Datas[name] = dict()
        self.update()
        self.current(len(self.Datas)-1)
        self.distributionObject.Load(self.Datas[name])

    def DeleteConfig(self):
        if len(self.Datas) == 0:
            return
        slc = str(self.selector.get())
        if messagebox.askyesno("Delete Config", 'Are you sure you want to delete "{}" ?'.format(slc)):
            del self.Datas[slc]
            self.update()
        if len(self.Datas) > 0:
            self.current(0)
            self.distributionObject.Load(self.Datas[self.selector.get()])
        else:
            self.selector_set("")
            self.distributionObject.Load(None)

    def Save(self, index=None):
        if index is None:
            index = self.selector.get()
        self.Datas[index] = self.distributionObject.Save()
        value = {"Datas": self.Datas}
        value["current"] = index
        return value
    
    def Load(self, value):
        if value is None:
            value = dict()
        if "Datas" in value:
            self.Datas = value["Datas"]
        else:
            self.Datas = dict()
        self.update()
        if "current" in value and value["current"] in self.Datas:
            self.selector_set(value["current"])
            self.distributionObject.Load(self.Datas[value["current"]])
        else:
            if len(self.Datas) > 0:
                self.current(0)
                self.distributionObject.Load(self.Datas[self.selector.get()])
            else:
                self.selector_set("")
                self.distributionObject.Load(None)
    def Reload(self):
        self.distributionObject.Load(self.Datas[self.selector.get()])

class DirectoryFile(Frame):
    def __init__(self, root, path, *args, **kwargs):
        Frame.__init__(self, root)
        self.path = path
        self.is_checked = IntVar()
        self.checkbutton = Checkbutton(self, text=self.path, variable=self.is_checked)
        self.checkbutton.grid(row=0, column=0, sticky=W)

    def destroy(self):
        self.checkbutton.destroy()
        Frame.destroy(self)

    def Save(self):
        return bool(self.is_checked.get())
    
    def Load(self, value):
        if type(value) == bool:
            self.is_checked.set(int(value))
        else:
            self.is_checked.set(0)

class DirectoryFileCombobox(Frame):
    def __init__(self, root, path, values, *args, **kwargs):
        Frame.__init__(self, root)
        self.path = path
        self.is_checked = IntVar()
        self.checkbutton = Checkbutton(self, text=self.path, variable=self.is_checked)
        self.checkbutton.grid(row=0, column=0, sticky=W)
        
        self.combobox = ttk.Combobox(self, state="readonly", values=tuple(values))
        self.combobox.grid(row=0, column=1, sticky=W)

    def destroy(self):
        self.checkbutton.destroy()
        Frame.destroy(self)

    def Save(self):
        data = {"is_checked": bool(self.is_checked.get()),
                "combobox": str(self.combobox.get())}
        return data
    
    def Load(self, value):
        if type(value) == dict:
            if "is_checked" in value:
                self.is_checked.set(int(value["is_checked"]))
            else:
                self.is_checked.set(0)
            if "combobox" in value:
                self.combobox.set(value["combobox"])
            else:
                self.combobox.set("")
        else:
            self.is_checked.set(0)
            self.combobox.set("")

class MyDirectoryFile(Frame):
    def __init__(self, root, filetypes=None, directoryFileType = DirectoryFile, directoryFileArgs = {}, *args, **kwargs):
        Frame.__init__(self, root)
        self.filetypes = filetypes
        self.directoryFileType = directoryFileType
        self.directoryFileArgs = directoryFileArgs
        self.path = MyEntry(self, "Directory")
        self.path.pack(side=TOP, fill=X)
        self.path.entry.bind("<Return>", self.UpdateDir)
        self.scrollbar = Scrollbar(self)
        self.scrollbar.pack(side=RIGHT, fill=Y)
        self.canvas = Canvas(self, yscrollcommand=self.scrollbar.set)
        self.canvas.pack(side=LEFT, fill=BOTH, expand=1)
        self.scrollbar.config(command=self.canvas.yview)
        #make frame for canvas
        self.frame = Frame(self.canvas)
        self.canvas.create_window((0,0), window=self.frame, anchor=NW)

        self.CheckButtons = []
        self.UpdateDir()

    def UpdateDir(self,Saveload = True, *args, **kwargs):
        if Saveload:
            data = self.Save()
        for i in self.CheckButtons:
            i.destroy()
        self.CheckButtons = []
        path = self.path.get()
        path = os.path.join(base_path, path)
        if path == None or path == "":
            return
        #check if path is a directory
        if not os.path.isdir(path):
            return
        self.files = []
        for root, dirs, files in os.walk(path):
            if root in self.files:
                continue
            if False:#IsSpriteFolder(root):
                self.files.append(root)
            else:
                for file in files:
                    if self.filetypes == None or os.path.splitext(file)[1].lower() in self.filetypes:
                        p = os.path.relpath(os.path.join(root, file), path)
                        self.files.append(p)
        for ind, file in enumerate(self.files):
            checkFile = self.directoryFileType(self.frame, file, **self.directoryFileArgs)
            checkFile.grid(row=ind, column=0, sticky=W)
            self.CheckButtons.append(checkFile)
        #update canvas
        self.frame.update_idletasks()
        self.canvas.config(scrollregion=self.canvas.bbox("all"))
        if Saveload:
            self.Load(data)

    def Save(self):
        data = {"path": self.path.get(),
                "files": [(i.Save(), i.path) for i in self.CheckButtons]}
        return data
    
    def Load(self, value):
        if type(value) == dict:
            if "path" in value:
                self.path.set(value["path"])
            if "files" in value:
                self.UpdateDir(Saveload = False)
                for data, file in value["files"]:
                    for i in self.CheckButtons:
                        if os.path.normpath(i.path) == os.path.normpath(file):
                            i.Load(data)
                            break

class MyScrollFrame(Frame):
    def __init__(self, root, *args, **kwargs):
        Frame.__init__(self, root)
        self.scrollbar = Scrollbar(self)
        self.scrollbar.pack(side=RIGHT, fill=Y)
        self.canvas = Canvas(self, yscrollcommand=self.scrollbar.set)
        self.canvas.pack(side=LEFT, fill=BOTH, expand=1)
        self.scrollbar.config(command=self.canvas.yview)
        #make frame for canvas
        self.frame = Frame(self.canvas)
        self.canvas.create_window((0,0), window=self.frame, anchor=NW)
        self.frame.bind("<Configure>", self.OnFrameConfigure)

    def OnFrameConfigure(self, event):
        self.canvas.config(scrollregion=self.canvas.bbox("all"))

    def Update(self):
        self.frame.update_idletasks()
        self.canvas.config(scrollregion=self.canvas.bbox("all"))


class MyButton(Button):
    def __init__(self, *args, **kwargs):
        command = kwargs.pop("command", None)
        Button.__init__(self, *args, **kwargs)
        self.commands = []
        if command is not None:
            self.commands.append(command)
        self.config(command=self.MultipleCommand)

    def AddCommand(self, command):
        self.commands.append(command)

    def MultipleCommand(self):
        for command in self.commands:
            command()

