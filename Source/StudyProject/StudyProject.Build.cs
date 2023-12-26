// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StudyProject : ModuleRules //필요한 레고들을 가져온다.
{
	public StudyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			// InitialModules
			"Core", "CoreUObject", "Engine", "InputCore",

			// JsonModules
			"Json", "JsonUtilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
