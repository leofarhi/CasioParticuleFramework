from Essentials import *
from BaseDistribution import *

class WindowsDistribution(BaseDistribution):
    NAME = "Windows"
    DEFINE = "DIST_WINDOWS"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        self.Makefile.includesMakefile.append(JoinPath(self.DistributionDir(), "Makefile"))
        self.Makefile.defines.append(WindowsDistribution.DEFINE)

    def OnAddGeneralTab(self):
        BaseDistribution.OnAddGeneralTab(self)
        self.TabGeneral.Simulator = MyCombobox(self.TabGeneral, name="Simulate a distribution", state="readonly")
        self.TabGeneral.Simulator.grid(row=4, column=0, sticky="ew")
        self.TabGeneral.Simulator.AfterLoadSystem = AfterLoadSystem(self.TabGeneral.Simulator.Save, self.TabGeneral.Simulator.Load)
        self.SaveLoadSystem.Add("TabGeneral.Simulator", self.TabGeneral.Simulator.AfterLoadSystem)
        self.TabGeneral.grid_rowconfigure(4, weight=0)
        self.TabGeneral.grid_rowconfigure(5, weight=1)
        self.TabGeneral.grid_columnconfigure(0, weight=1)

    def OnCloneMakefile(self, clone):
        for dist in self.Make.Distributions:
            if dist.NAME in ["Windows", "Linux"]:
                if self.TabGeneral.Simulator.get() == dist.NAME:
                    define = "SIMU_"+dist.DEFINE
                    print("Simulator", dist.NAME, define)
                    clone.defines.append(define)
        return clone
    
    def AfterLoad(self):
        simulator_values = [i.NAME for i in self.Make.Distributions]
        if "Windows" in simulator_values:
            simulator_values.remove("Windows")
        if "Linux" in simulator_values:
            simulator_values.remove("Linux")
        simulator_values.insert(0, "Disabled")
        self.TabGeneral.Simulator.SetValues(simulator_values)
        self.TabGeneral.Simulator.AfterLoadSystem.AfterLoad()

def Load():
    return WindowsDistribution