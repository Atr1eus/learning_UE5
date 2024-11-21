// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class week1 : ModuleRules
{
	public week1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
