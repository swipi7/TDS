// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class My_TDS_GameProject : ModuleRules
{
	public My_TDS_GameProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "PhysicsCore" });

		PublicIncludePaths.AddRange(new string[]
		{"My_TDS_GameProject",
		"My_TDS_GameProject/Character",
		"My_TDS_GameProject/FuncLibrary",
		"My_TDS_GameProject/Game"
		});
    }
}
