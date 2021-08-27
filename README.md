# UE4NoirQBudgetBridge

A "Budget" Bridge between Modo to UE4.

Open the Kit Layout Export form from the kits menu. Export the file to a json file at the project root of your game, deifned with the game tools project dir.
It will read any user channel on the modo scene items "noirqRedirect" that is a string.

In Unreal, there's a button at the top bar. Press that, then load layout file. By Default that looks in GameRoot/BridgeFile.json.
(this can be changed in the proect settings). This will then try and load any of the found redirects into the scene
at the location they are in modo.

You can use this to drive locations of blueprints / skeletal meshes / existing static meshes using the layout tools from Modo.

This will attmept any locator type item in the scene (i.e basically everything), so you could have just locators represing the positions if required.
It will also export replciators.

Tested with modo 15.1 and Unreal 4.27