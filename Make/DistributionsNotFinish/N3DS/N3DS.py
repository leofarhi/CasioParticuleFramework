from Essentials import *
from BaseDistribution import *

class N3dsDistribution(BaseDistribution):
    NAME = "Nintendo 3DS"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        

def Load():
    return N3dsDistribution