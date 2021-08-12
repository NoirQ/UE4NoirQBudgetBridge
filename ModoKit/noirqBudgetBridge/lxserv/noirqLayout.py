# !/usr/bin/env python
import modo
import json
import lx
import lxu
import lxifc
import os

class replicatorVisitor(lxifc.Visitor):
    def __init__(self):
        self.repEnum = None
        self.allItems = []

    def vis_Evaluate(self):
        pos = self.repEnum.Position()
        ori = self.repEnum.Orientation()
        mOri = modo.Matrix4(ori)
        mOri.transpose() # this comes out flipped for some reason, so flip it back.
        m = modo.Matrix4(mOri, pos)
        self.allItems.append(m)


class ExportLayoutCMD(lxu.command.BasicCommand):
    def __init__(self):
        lxu.command.BasicCommand.__init__(self)
        self.dyna_Add ('selected', lx.symbol.sTYPE_BOOLEAN)
        self.basic_SetFlags (0, lx.symbol.fCMDARG_OPTIONAL)
    def cmd_Flags(self):
        return 0
    def basic_Enable(self, msg):
        return True



    def handleReplicator(self, item):
        srvScene = lx.service.Scene()
        print item
        e = srvScene.GetReplicatorEnumerator(item)
        print e
        srvSel = lx.service.Selection()
        scene = lxu.select.SceneSelection().current()
        time = srvSel.GetTime ()
        read = lx.object.ChannelRead (scene.Channels (None, time))

        v = replicatorVisitor()
        v.repEnum = e
        e.Enumerate(v, read, False)
        return v.allItems

    def basic_Execute(self, msg, flags):

        selected = self.dyna_Bool(0)

        srvScn = lx.service.Scene()
        scene = lxu.select.SceneSelection().current()
        all_items = []
        types = []
        itype = "locator;replicator"
        if isinstance(itype, str):
            allUserTypes = []
            allUserTypes = itype.split(";")     
            for uType in allUserTypes:   
                itype = srvScn.ItemTypeLookup(uType)
                types.append(itype)
        if itype == None:
            types = []
            for i in range(srvScn.ItemTypeCount()):
                types.append(srvScn.ItemTypeByIndex(i) )

        types.append(0)

        item_types = lx.object.storage()
        item_types.setType('i')
        item_types.set(types)

        srvSel = lx.service.Selection()

        idents = set()
        item_count = scene.ItemCountByTypes(item_types)
        
        for i in range(item_count):
            bAdd = False
            item = scene.ItemByIndexByTypes(item_types, i)
            ident = item.Ident()
            if selected:
                spTrans = lx.object.ItemPacketTranslation(srvSel.Allocate(lx.symbol.sSELTYP_ITEM))
                spType = srvSel.LookupType(lx.symbol.sSELTYP_ITEM)
                pkt = spTrans.Packet(item)
                if srvSel.Test(spType, pkt):
                    bAdd  = True
            else:
                bAdd = True
            if bAdd:
                if ident not in idents:
                    idents.add(ident)
                    all_items.append(item)

        if len(all_items) == 0:
            return

        time = srvSel.GetTime ()
        read = lx.object.ChannelRead (scene.Channels (None, time))

        layoutDict = {
            "items" : []
        }

        for item in all_items:
            # todo: move to raw sdk instead of td.
            localItems = []
            worldId = item.ChannelLookup(lx.symbol.sICHAN_XFRMCORE_WORLDMATRIX)
            itype = srvScn.ItemTypeLookup("replicator")
            tdItem = modo.Item(item)
            if item.IsA(itype):
                localItems = self.handleReplicator(item)
            else:
                matrixObject = tdItem.channel('worldMatrix').get()
                matrix = modo.Matrix4(matrixObject)
                localItems.append(matrix)
            replicatorIdx = 0
            for matrix in localItems:
                ueMat = convertToUEMatrix(matrix)
                assetRedirect = ""
                try:
                    assetRedirectId = item.ChannelLookup("noirqRedirect")
                    assetRedirect = read.String(item, assetRedirectId)
                except:
                    print ("no redirect found")

                name = item.UniqueName()
                if replicatorIdx > 0:
                    name = "{0}_{1}".format(name, replicatorIdx)
                itemDict = {
                    "item" : name,
                    "matrix" : tuple(ueMat),
                    "redirect" : assetRedirect
                    # can it do the hierarchy? Do I really need to?
                    #"children" : []

                }

                layoutDict["items"].append(itemDict)
                replicatorIdx = replicatorIdx + 1


        # Serializing json 
        json_object = json.dumps(layoutDict, indent = 4)
          
        # Writing to sample.json
        outpath = lx.eval("user.value noirqLayout.filePath ?")
        asPath = os.path.join(outpath)

        with open(asPath, "w") as outfile:
            outfile.write(json_object)


        
        

def convertToUEMatrix(inMatrix4):
    
    UEMatrix = modo.Matrix4(inMatrix4)
    # https://stackoverflow.com/questions/1263072/changing-a-matrix-from-right-handed-to-left-handed-coordinate-system
    convertMatrix = modo.Matrix4()
    convertMatrix[0][0] = 1
    convertMatrix[0][1] = 0
    convertMatrix[0][2] = 0
    convertMatrix[0][3] = 0
    convertMatrix[1][0] = 0
    convertMatrix[1][1] = 0
    convertMatrix[1][2] = 1
    convertMatrix[1][3] = 0
    convertMatrix[2][0] = 0
    convertMatrix[2][1] = 1
    convertMatrix[2][2] = 0
    convertMatrix[2][3] = 0
    convertMatrix[3][0] = 0
    convertMatrix[3][1] = 0
    convertMatrix[3][2] = 0
    convertMatrix[3][3] = 1

    UEMatrix = convertMatrix * inMatrix4 * convertMatrix
    # Robbed from UnrealHelper.cpp in the Old Modo Bridge unreal plugin. Didn't work
    #for i in range(4):
    #    if (i == 1):
    #        UEMatrix[i][0] = -inMatrix4[i][0];
    #        UEMatrix[i][1] = inMatrix4[i][1];
    #        UEMatrix[i][2] = -inMatrix4[i][2];
    #        UEMatrix[i][3] = -inMatrix4[i][3];
    #    else:
    #        UEMatrix[i][0] = inMatrix4[i][0];
    #        UEMatrix[i][1] = -inMatrix4[i][1];
    #        UEMatrix[i][2] = inMatrix4[i][2];
    #        UEMatrix[i][3] = inMatrix4[i][3];

    #UEMatrix.transpose()

    pos = inMatrix4.position

    # swap y + z
    x = pos[0]
    y = pos[2]
    z = pos[1]

    newPos = [x,y,z]
    
    # swap to UE units
    for i in range(3):
        newPos[i] = newPos[i] * 100  
    
    UEMatrix.position = tuple(newPos)
            
    return UEMatrix



lx.bless(ExportLayoutCMD, "noirqLayout.save")
    


