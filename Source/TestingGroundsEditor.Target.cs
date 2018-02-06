// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TestingGroundsEditorTarget : TargetRules
{
	public TestingGroundsEditorTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "TestingGrounds" });
    }
}
