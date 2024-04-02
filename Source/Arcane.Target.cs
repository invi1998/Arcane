// Copyright INVI1998

using UnrealBuildTool;
using System.Collections.Generic;

public class ArcaneTarget : TargetRules
{
	public ArcaneTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Arcane" } );
	}
}
