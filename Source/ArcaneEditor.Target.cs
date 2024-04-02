// Copyright INVI1998

using UnrealBuildTool;
using System.Collections.Generic;

public class ArcaneEditorTarget : TargetRules
{
	public ArcaneEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Arcane" } );
	}
}
