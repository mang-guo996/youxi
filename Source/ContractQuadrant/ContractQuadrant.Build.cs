// Copyright (C) 2025 契约象限. All Rights Reserved.

using UnrealBuildTool;

public class ContractQuadrant : ModuleRules
{
	public ContractQuadrant(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayTags"
		});
	}
}
