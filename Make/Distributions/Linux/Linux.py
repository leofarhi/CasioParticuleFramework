from Essentials import *
from BaseDistribution import *
from Distributions.Windows.Windows import WindowsDistribution

class LinuxDistribution(WindowsDistribution):
    NAME = "Linux"
    def __init__(self, Make, MainFrame):
        WindowsDistribution.__init__(self, Make, MainFrame)
        self.Makefile.defines.remove("DIST_WINDOWS")
        self.Makefile.defines.append("DIST_LINUX")
        

def Load():
    return LinuxDistribution