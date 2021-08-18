//----
// InteractML - University Of Arts London
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
FText FInteractMLTrainingSetActions::GetName() const
{
	return LOCTEXT("TrainingSetAssetName", "Training Set");
}

FColor FInteractMLTrainingSetActions::GetTypeColor() const
{
	return FColor::FromHex("74DF84"); //InteractML Unity plugin training set icon colour
}

UClass* FInteractMLTrainingSetActions::GetSupportedClass() const
{
	return UInteractMLTrainingSet::StaticClass();
}

uint32 FInteractMLTrainingSetActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

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

