//----
// InteractML - University Of Arts London
//----

using UnrealBuildTool;
using System.IO;
using System.Diagnostics;

public class InteractMLScripting : ModuleRules
{
	public InteractMLScripting(ReadOnlyTargetRules Target) : base(Target)
	{
		//normal
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(
			new string[] 
			{
			}
			);			

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				//higher level engine modules
				"Core",
				"Engine",
				"UnrealEd",
				"AssetTools",
				"Kismet",	//for SContentReference
				"ToolMenus", //for UToolMenu (node context menus)

				// ... add other public dependencies that you statically link with here ...
				"InteractML"
			}
			);		
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{			
				//fundamental unreal modules
				"CoreUObject",
				"SlateCore",
				"Slate",
				"BlueprintGraph",	//UK2Node
				"KismetCompiler",	//FKismetCompilerContext
			}
			);		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	
	}

}
