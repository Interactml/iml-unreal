//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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

// display name of asset type
//
FText FInteractMLClassificationModelActions::GetName() const
{
	return LOCTEXT("ClassificationModeAssetName", "Classification Model");
}

// 'colour' of asset (used on various UI flashes to help visual identification)
//
FColor FInteractMLClassificationModelActions::GetTypeColor() const
{
	return FColor::FromHex("5EB3F9"); //InteractML Unity plugin classification icon colour
}

// actual asset class we represent
//
UClass* FInteractMLClassificationModelActions::GetSupportedClass() const
{
	return UInteractMLClassificationModel::StaticClass();
}

// asset categorisation (comes from IML editor module startup)
//
uint32 FInteractMLClassificationModelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}


////////////////////////////// REGRESSION MODEL /////////////////////////////

// IAssetTypeActions Implementation

// display name of asset type
//
FText FInteractMLRegressionModelActions::GetName() const
{
	return LOCTEXT("RegressionModeAssetName", "Regression Model");
}

// 'colour' of asset (used on various UI flashes to help visual identification)
//
FColor FInteractMLRegressionModelActions::GetTypeColor() const
{
	return FColor::FromHex("F6C46F"); //InteractML Unity plugin regression icon colour
}

// actual asset class we represent
//
UClass* FInteractMLRegressionModelActions::GetSupportedClass() const
{
	return UInteractMLRegressionModel::StaticClass();
}

// asset categorisation (comes from IML editor module startup)
//
uint32 FInteractMLRegressionModelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}


////////////////////////////// DTW MODEL /////////////////////////////

// IAssetTypeActions Implementation

// display name of asset type
//
FText FInteractMLDynamicTimewarpModelActions::GetName() const
{
	return LOCTEXT("DynamicTimewarpModeAssetName", "DTW Model");
}

// 'colour' of asset (used on various UI flashes to help visual identification)
//
FColor FInteractMLDynamicTimewarpModelActions::GetTypeColor() const
{
	return FColor::FromHex("BB6BD9"); //InteractML Unity plugin DTW icon colour
}

// actual asset class we represent
//
UClass* FInteractMLDynamicTimewarpModelActions::GetSupportedClass() const
{
	return UInteractMLDynamicTimeWarpModel::StaticClass();
}

// asset categorisation (comes from IML editor module startup)
//
uint32 FInteractMLDynamicTimewarpModelActions::GetCategories()
{
	return RegisteredCategoryHandle;
}


#undef LOCTEXT_NAMESPACE

