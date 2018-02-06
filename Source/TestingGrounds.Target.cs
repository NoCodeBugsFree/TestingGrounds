// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TestingGroundsTarget : TargetRules
{
	public TestingGroundsTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Game;

        ExtraModuleNames.AddRange(new string[] { "TestingGrounds" });
        
    }
}
