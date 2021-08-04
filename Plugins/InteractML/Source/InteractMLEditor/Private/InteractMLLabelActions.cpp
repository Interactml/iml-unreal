//----
// InteractML - University Of Arts London
//----

//main
#include "InteractMLLabelActions.h"

//unreal
#include "BlueprintEditorModule.h"

//module
#include "InteractMLLabel.h"

//module editor


#define LOCTEXT_NAMESPACE "InteractML"


////////////////////////////// LABEL /////////////////////////////

// IAssetTypeActions Implementation
FText FInteractMLLabelActions::GetName() const
{
	return LOCTEXT("LabelAssetName", "Label Type");
}

FColor FInteractMLLabelActions::GetTypeColor() const
{
	return FColor::FromHex("B8BBFA"); //InteractML Violent
}

UClass* FInteractMLLabelActions::GetSupportedClass() const
{
	return UInteractMLLabel::StaticClass();
}

uint32 FInteractMLLabelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

void FInteractMLLabelActions::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor )
{
	for (UObject* obj : InObjects)
	{
		const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
		
		FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>( "Kismet" );
		
		for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
		{
			if (UInteractMLLabel* UDStruct = Cast<UInteractMLLabel>(*ObjIt))
			{
				BlueprintEditorModule.CreateUserDefinedStructEditor(Mode, EditWithinLevelEditor, UDStruct);
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE

