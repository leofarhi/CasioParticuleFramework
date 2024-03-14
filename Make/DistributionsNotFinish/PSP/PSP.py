from Essentials import *
from BaseDistribution import *

class PspDistribution(BaseDistribution):
    NAME = "PSP"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        
    def OnAddBottomButton(self):
        self.AddBottomButton("Run", None)
        self.AddBottomButton("Clean", None)
        self.AddBottomButton("Rebuild", None)

def Load():
    return PspDistribution