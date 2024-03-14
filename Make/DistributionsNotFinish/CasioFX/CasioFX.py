from Essentials import *
from BaseDistribution import *

class CasioFxDistribution(BaseDistribution):
    NAME = "Casio FX"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        

def Load():
    return CasioFxDistribution