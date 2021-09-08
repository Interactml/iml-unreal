//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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

// display name of asset type
//
FText FInteractMLLabelActions::GetName() const
{
	return LOCTEXT("LabelAssetName", "Label Type");
}

// 'colour' of asset (used on various UI flashes to help visual identification)
//
FColor FInteractMLLabelActions::GetTypeColor() const
{
	return FColor::FromHex("B8BBFA"); //InteractML Violent
}

// actual asset class we represent
//
UClass* FInteractMLLabelActions::GetSupportedClass() const
{
	return UInteractMLLabel::StaticClass();
}

// asset categorisation (comes from IML editor module startup)
//
uint32 FInteractMLLabelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

// open an explicit editor for this asset instead of the default details panel
// NOTE: copied from FAssetTypeActions_Struct::OpenAssetEditor as we want the same functionality
//
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

