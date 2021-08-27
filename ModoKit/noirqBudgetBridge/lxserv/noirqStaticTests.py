#python

import modo

import lx
import lxifc
import lxu

import lxu.command as SAC

class noirqLayoutMissingChannel(SAC.StaticAnalysisCommand):
    def __init__(self):
        SAC.StaticAnalysisCommand.__init__ (self)
        self.dyna_Add("redirectValue", lx.symbol.sTYPE_STRING)
        self.dyna_SetFlags(self.DefaultArgumentsCount+1, lx.symbol.fCMDARG_OPTIONAL)
        
    def sa_Name(self):
        return "Budget Bridge - Missing Channel"

    def sa_Category(self):
        return "Budget Bridge"

    def sa_Test(self, item=None):  
        myItem = modo.Item(item)
        if myItem.superType == "locator" or myItem.type == "replicator":
            channel = myItem.channel("noirqRedirect")
            if channel is None:
                return "missing channel"
            if channel.get() is "":
                return "Channel is blank. Needs a reference"
        return ""

    def sa_Fix(self, item=None):
        myItem = modo.Item(item)
        if myItem.superType == "locator" or myItem.type == "replicator":
            channel = myItem.channel("noirqRedirect")
            if channel is None:
                lx.eval("noirqLayout.addChannel item:{}".format(myItem.Ident()))
            
        
SAC.RegisterStaticAnalysisTest(noirqLayoutMissingChannel, "noirqLayout.saTest")