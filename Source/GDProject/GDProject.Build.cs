// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GDProject : ModuleRules
{
	public GDProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
			"NavigationSystem", 
			"Niagara", 
			"AIModule", 
			"ModelViewViewModel",
			"InventoryModule",
			"ToolModule"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
