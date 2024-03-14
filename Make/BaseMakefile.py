from Essentials import *

class Makefile:
    def __init__(self, filename):
        self.filename = filename
        self.includesMakefile = []
        self.sources = []
        self.libs = ["Libs/Essential/List","Libs/Essential/Tuple","Libs/Essential/Dict"]
        self.defines = []

        self.LDLIBS = ""
        self.CFLAGS = ""

        self.DEBUG = False

        self.TITLE = "Engine"
        self.CC = "gcc"

        self.path_dirname = os.path.relpath(base_path, os.path.dirname(self.filename))

        self.OUTPUT = JoinPath(self.path_dirname,"Build","bin")
        self.BUILD = JoinPath(self.path_dirname,"Build","build")

        self.LibsDir = JoinPath(self.path_dirname,"Libs")

    def MakeFolders(self):
        CreateDir(JoinPath(base_path,"Build"))
        CreateDir(JoinPath(base_path,"Build","bin"))
        CreateDir(JoinPath(base_path,"Build","build"))

    def Write(self):
        ReplaceLinesInFile(self.filename, "TITLE_FIELD", "\nTITLE = " + self.TITLE + "\n")
        ReplaceLinesInFile(self.filename, "COMPILATOR_FIELD", "\nCC = " + self.CC + "\n")
        ReplaceLinesInFile(self.filename, "OUTPUT_FIELD", "\nOUTPUT = " + normalize_path(self.OUTPUT) + "\n")
        ReplaceLinesInFile(self.filename, "BUILD_FIELD", "\nBUILD = " + normalize_path(self.BUILD) + "\n")
        ReplaceLinesInFile(self.filename, "LIBS_DIR_FIELD", "\nLIBS_DIR = " + normalize_path(self.LibsDir) + "\n")
        sources = []
        for source in self.sources:
            sources += GetAllSubFolders(source)
        sources = [normalize_path(os.path.relpath(s, os.path.dirname(self.filename))) for s in sources]
        SourceDirs = "\nSourceDirs += "+ " ".join(sources) + "\n"
        ReplaceLinesInFile(self.filename, "SOURCE_DIR_FIELD", SourceDirs)
        if self.DEBUG:
            ReplaceLinesInFile(self.filename, "DEBUG_FIELD", "\nCFLAGS += -DDEBUG_MODE\n")
        else:
            ReplaceLinesInFile(self.filename, "DEBUG_FIELD", "\n\n")
        includesMakefile = [normalize_path(os.path.relpath(i, os.path.dirname(self.filename))) for i in self.includesMakefile]
        includeMakefile = "\n".join([f"include {i}" for i in includesMakefile]) + "\n"
        ReplaceLinesInFile(self.filename, "INCLUDE_MAKEFILE", "\n"+includeMakefile)
        LDLIBS = "\nLDLIBS += " + " ".join([f"-L {normalize_path(os.path.relpath(i, os.path.dirname(self.filename)))}" for i in self.libs])
        CFLAGS = "\nCFLAGS += " + " ".join([f"-I {normalize_path(os.path.relpath(i, os.path.dirname(self.filename)))}" for i in self.libs])
        LDLIBS += " " + self.LDLIBS + "\n"
        CFLAGS += " " + self.CFLAGS + "\n"
        CFLAGS += "\nCFLAGS += " + " ".join(["-D"+i for i in self.defines]) + "\n"
        ReplaceLinesInFile(self.filename, "CFLAGS_AND_LDLIBS_FIELD", CFLAGS + LDLIBS)

    def Clone(self):
        clone = Makefile(self.filename)
        #get all the attributes
        for attr in dir(self):
            if not attr.startswith("__") and not callable(getattr(self, attr)):
                attrValue = getattr(self, attr)
                #print(attr, attrValue)
                if isinstance(attrValue, list):
                    attrValue = attrValue.copy()
                setattr(clone, attr, attrValue)
        return clone