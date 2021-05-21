//----
// InteractML - University Of Arts London
//----

using UnrealBuildTool;
using System.IO;

public class InteractMLEditor : ModuleRules
{
	public InteractMLEditor(ReadOnlyTargetRules Target) : base(Target)
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
				"Core",
				"Projects",
				"UnrealEd",
				"Engine",
				"AssetTools",
				"EditorStyle",
				"Kismet",	//for SContentReference
				
				// ... add other public dependencies that you statically link with here ...
				"InteractML",
				"InteractMLScripting"
			}
			);		
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Slate",
				"SlateCore",
				"PropertyEditor",
				"EditorWidgets",
				"InputCore",
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
