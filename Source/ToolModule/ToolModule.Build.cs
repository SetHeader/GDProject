using UnrealBuildTool;

public class ToolModule : ModuleRules
{
    public ToolModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                // "Engine",
                // "Slate",
                // "SlateCore"
            }
        );
    }
}