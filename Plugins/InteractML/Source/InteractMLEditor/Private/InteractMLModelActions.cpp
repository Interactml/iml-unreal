//----
// InteractML - University Of Arts London
//----

//main
#include "InteractMLModelActions.h"

//unreal

//module
#include "Models/InteractMLClassificationModel.h"
#include "Models/InteractMLRegressionModel.h"
#include "Models/InteractMLDynamicTimewarpModel.h"

//module editor


#define LOCTEXT_NAMESPACE "InteractML"


////////////////////////////// CLASSIFICATION MODEL /////////////////////////////

// IAssetTypeActions Implementation
FText FInteractMLClassificationModelActions::GetName() const
{
	return LOCTEXT("ClassificationModeAssetName", "Classification Model");
}

FColor FInteractMLClassificationModelActions::GetTypeColor() const
{
	return FColor::FromHex("5EB3F9"); //InteractML Unity plugin classification icon colour
}

UClass* FInteractMLClassificationModelActions::GetSupportedClass() const
{
	return UInteractMLClassificationModel::StaticClass();
}

uint32 FInteractMLClassificationModelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

#if 0
void FInteractMLClassificationModelActions::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor )
{
	for (UObject* obj : InObjects)
	{
		UInteractMLClassificationModel* model = Cast<UInteractMLClassificationModel>(obj);
		if (model)
		{
			//open panel
			TSharedRef< FModelEditor > new_model_editor( new FModelEditor() );
			new_model_editor->InitTrainingSetEditor( EToolkitMode::Standalone, EditWithinLevelEditor, model );
		}
	}
}
#endif


////////////////////////////// REGRESSION MODEL /////////////////////////////

// IAssetTypeActions Implementation
FText FInteractMLRegressionModelActions::GetName() const
{
	return LOCTEXT("RegressionModeAssetName", "Regression Model");
}

FColor FInteractMLRegressionModelActions::GetTypeColor() const
{
	return FColor::FromHex("F6C46F"); //InteractML Unity plugin regression icon colour
}

UClass* FInteractMLRegressionModelActions::GetSupportedClass() const
{
	return UInteractMLRegressionModel::StaticClass();
}

uint32 FInteractMLRegressionModelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

#if 0
void FInteractMLRegressionModelActions::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor )
{
	for (UObject* obj : InObjects)
	{
		UInteractMLRegressionModel* model = Cast<UInteractMLRegressionModel>(obj);
		if (model)
		{
			//open panel
			TSharedRef< FModelEditor > new_model_editor( new FModelEditor() );
			new_model_editor->InitTrainingSetEditor( EToolkitMode::Standalone, EditWithinLevelEditor, model );
		}
	}
}
#endif


////////////////////////////// DTW MODEL /////////////////////////////

// IAssetTypeActions Implementation
FText FInteractMLDynamicTimewarpModelActions::GetName() const
{
	return LOCTEXT("DynamicTimewarpModeAssetName", "DTW Model");
}

FColor FInteractMLDynamicTimewarpModelActions::GetTypeColor() const
{
	return FColor::FromHex("BB6BD9"); //InteractML Unity plugin DTW icon colour
}

UClass* FInteractMLDynamicTimewarpModelActions::GetSupportedClass() const
{
	return UInteractMLDynamicTimeWarpModel::StaticClass();
}

uint32 FInteractMLDynamicTimewarpModelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

#if 0
void FInteractMLDynamicTimewarpModelActions::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor )
{
	for (UObject* obj : InObjects)
	{
		UInteractMLDynamicTimeWarpModel* model = Cast<UInteractMLDynamicTimeWarpModel>(obj);
		if (model)
		{
			//open panel
			TSharedRef< FModelEditor > new_model_editor( new FModelEditor() );
			new_model_editor->InitTrainingSetEditor( EToolkitMode::Standalone, EditWithinLevelEditor, model );
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE

