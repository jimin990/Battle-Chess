using UnrealBuildTool;
using System.Collections.Generic;

public class ChessServerTarget : TargetRules
{
    public ChessServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.Add("Chess");
    }
}