// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpTarget : TargetRules
{
	public ShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		bOverrideBuildEnvironment = true;
		AdditionalCompilerArguments = "-Wno-unused-but-set-variable";
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
	}
}
