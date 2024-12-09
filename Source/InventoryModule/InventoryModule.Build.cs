using UnrealBuildTool;

public class InventoryModule : ModuleRules
{
    public InventoryModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                // "Slate",
                // "SlateCore",
                "GameplayAbilities", 
                "GameplayTags",
                "ToolModule",
                "DeveloperSettings"
            }
        );
    }
}