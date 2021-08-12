// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class NoirQBudgetBridge : ModuleRules
	{
		public NoirQBudgetBridge(ReadOnlyTargetRules Target) : base (Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"Developer/NoirQBudgetBridge/Private",
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
                    "Engine",
                    "LevelEditor",
                    "CoreUObject",
                    "UMG",
                    "EditorStyle",
                    "Projects",
                    "DesktopPlatform",
                    "Sockets",
                    "Networking",
                    "UnrealEd",
					"Json",
                    "JsonUtilities"
					// ... add other public dependencies that you statically link with here ...
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Slate",
                    "SlateCore",
                    "DesktopPlatform",
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
                    
				}
				);
		}
	}
}