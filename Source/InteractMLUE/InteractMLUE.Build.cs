// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class InteractMLUE : ModuleRules
{
	public InteractMLUE(ReadOnlyTargetRules Target) : base(Target)
	{
		string CurDirectory = Path.Combine(Directory.GetCurrentDirectory(), "../");

		string RapDirectory = Path.Combine(CurDirectory, "/Plugins/");

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities" });

		PublicDelayLoadDLLs.Add("RapidLibPlugin.dll");

		PublicAdditionalLibraries.Add(Path.Combine(RapDirectory, "RapidLibPlugin.lib"));

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
