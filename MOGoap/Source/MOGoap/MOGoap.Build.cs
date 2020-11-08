// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MOGoap : ModuleRules
{
	public MOGoap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.NoSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
