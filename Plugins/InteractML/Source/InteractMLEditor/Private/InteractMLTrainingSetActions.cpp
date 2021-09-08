//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

//main
#include "InteractMLTrainingSetActions.h"

//unreal

//module
#include "InteractMLTrainingSet.h"
#include "InteractMLTrainingSetEditor.h"


//module editor


#define LOCTEXT_NAMESPACE "InteractML"

// IAssetTypeActions Implementation

// display name of asset type
//
FText FInteractMLTrainingSetActions::GetName() const
{
	return LOCTEXT("TrainingSetAssetName", "Training Set");
}

// 'colour' of asset (used on various UI flashes to help visual identification)
//
FColor FInteractMLTrainingSetActions::GetTypeColor() const
{
	return FColor::FromHex("74DF84"); //InteractML Unity plugin training set icon colour
}

// actual asset class we represent
//
UClass* FInteractMLTrainingSetActions::GetSupportedClass() const
{
	return UInteractMLTrainingSet::StaticClass();
}

// asset categorisation (comes from IML editor module startup)
//
uint32 FInteractMLTrainingSetActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

// open our custom editor for training sets instead of the default details panel
//
void FInteractMLTrainingSetActions::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor )
{
	for (UObject* obj : InObjects)
	{
		UInteractMLTrainingSet* training_set = Cast<UInteractMLTrainingSet>(obj);
		if (training_set)
		{
			//open panel
			TSharedRef< FTrainingSetEditor > new_training_set_editor( new FTrainingSetEditor() );
			new_training_set_editor->InitTrainingSetEditor( EToolkitMode::Standalone, EditWithinLevelEditor, training_set );
		}
	}
}


#undef LOCTEXT_NAMESPACE

