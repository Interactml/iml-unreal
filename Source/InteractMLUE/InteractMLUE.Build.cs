// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class InteractMLUE : ModuleRules
{
	public InteractMLUE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
	
		AddRapidLibLibrary( Target );

		string CurDirectory = Path.GetFullPath( Path.Combine(ModuleDirectory, "..", ".." ) );

		string RapDirectory = Path.Combine(CurDirectory, "Plugins", "RapidLib");

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities" });

		PublicDelayLoadDLLs.Add("RapidLibPlugin.dll");

		//PublicAdditionalLibraries.Add(Path.Combine(RapDirectory, "RapidLibPlugin.lib"));

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}



	
	/// <summary>
	/// work out where RapidLib is and add lib ref and include paths needed
	/// </summary>
	/// <param name="Target"></param>
	void AddRapidLibLibrary( ReadOnlyTargetRules Target )
	{
		string module_path = ModuleDirectory;
		string rapidlib_path = Path.GetFullPath( Path.Combine( module_path, "..", "..", "Plugins", "InteractML", "Source", "3rdParty", "RapidLib" ) );
		string include_dir = Path.Combine( rapidlib_path, "src" );
		string lib_dir = Path.Combine( rapidlib_path, "lib" );

		//config
		bool is_lib_supported = false;
		if((Target.Platform==UnrealTargetPlatform.Win64))// || (Target.Platform==UnrealTargetPlatform.Win32))
		{
			//platform variants
			string platform_string = "x64"; 	//NOTE: unlike x64, the Win32 platform is called "Win32"

			//configuration
			string config_string; //shipping/test has none
			switch(Target.Configuration)
			{
				case UnrealTargetConfiguration.Debug:
				case UnrealTargetConfiguration.DebugGame:
					//full debug can't currently be supported (Unreal still links the release CRT)
					//instead, the debug build uses release CRL but is still unoptimised
				case UnrealTargetConfiguration.Development:
					config_string = "Debug";
					break;
				case UnrealTargetConfiguration.Shipping:
					config_string = "Release";
					break;
				default:
					throw new System.ApplicationException("Unsupported RapidLib build configuration "+Target.Configuration );
			}

			//narrow down location
			lib_dir = Path.Combine( lib_dir, platform_string, config_string );
			is_lib_supported = true;
		}

		if(is_lib_supported)
		{
			string lib_path = Path.Combine( lib_dir, "RapidLib.lib" );

			PublicAdditionalLibraries.Add( lib_path );
			PublicIncludePaths.Add( include_dir );
		}
	}
}
