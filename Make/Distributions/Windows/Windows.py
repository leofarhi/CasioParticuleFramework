from Essentials import *
from BaseDistribution import *

class WindowsDistribution(BaseDistribution):
    NAME = "Windows"
    def __init__(self, Make, MainFrame):
        BaseDistribution.__init__(self, Make, MainFrame)
        self.Makefile.includesMakefile.append(JoinPath(self.DistributionDir(), "Makefile"))
        self.Makefile.defines.append("DIST_WINDOWS")
        self.OnAddSimulatorTab()

    def OnAddSimulatorTab(self):
        self.TabSimulator = self.AddTab("Simulator")
        self.TabSimulator.Simulator = MyScrollFrame(self.TabSimulator)
        self.TabSimulator.Simulator.pack(side=TOP, fill=BOTH, expand=1)

        self.TabGeneral.Simulator = MyCombobox(self.TabGeneral, name="Simulator choice", state="readonly")
        self.TabGeneral.Simulator.grid(row=4, column=0, sticky="ew")
        self.TabGeneral.Simulator.AfterLoadSystem = AfterLoadSystem(self.TabGeneral.Simulator.Save, self.TabGeneral.Simulator.Load)
        self.SaveLoadSystem.Add("TabGeneral.Simulator", self.TabGeneral.Simulator.AfterLoadSystem)
        self.TabGeneral.grid_rowconfigure(4, weight=0)
        self.TabGeneral.grid_rowconfigure(5, weight=1)
        self.TabGeneral.grid_columnconfigure(0, weight=1)
    
    def AfterLoad(self):
        simulator_values = [i.NAME for i in self.Make.Distributions]
        self.TabGeneral.Simulator.SetValues(simulator_values)
        self.TabGeneral.Simulator.AfterLoadSystem.AfterLoad()

def Load():
    return WindowsDistribution