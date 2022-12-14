// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PuzzlePlatforms : ModuleRules
{
	public PuzzlePlatforms(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem" , "OnlineSubsystemSteam" });

        PublicIncludePaths.AddRange(new string[] {
        "PuzzlePlatforms/Public/GameInstances",
        "PuzzlePlatforms/Public/GameModes",
        "PuzzlePlatforms/Public/MenuSystem",
        "PuzzlePlatforms/Public/Interfaces"
        });


    }
}
