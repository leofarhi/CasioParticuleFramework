from Essentials import *
from BaseDistribution import *

class NdsDistribution(BaseDistribution):
    NAME = "Nintendo DS"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        

def Load():
    return NdsDistribution