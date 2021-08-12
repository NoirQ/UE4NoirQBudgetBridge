# UE4NoirQBudgetBridge

A "Budget" Bridge bewteen Modo to UE4.

Open the Kit Layout Export form from the kits menu. Export the file to a json file somewhere on your machine.
It will read any user channel on the modo scene items "noirqRedirect" that is a string. It wants this to be the copied 
reference from Unreal, minus the quotes and the type at the start. (eg G:/Game/Content/MyStaticMesh.MyStaticMesh)

In Unreal, there's a button at the top bar. Press that, then load layout file. By Default that looks in Game/BudgetBridge/BridgeFile.json.
(this can be changed in the proect settings). This will then try and load any of the found redirects into the scene
at the location they are in modo.
