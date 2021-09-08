//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

//main
#include "InteractMLModelFactory.h"

//unreal

//module
#include "InteractMLClassificationModel.h"
#include "InteractMLRegressionModel.h"
#include "InteractMLDynamicTimewarpModel.h"

//module editor


#define LOCTEXT_NAMESPACE "InteractML"

////////////////////////////// CLASSIFICATION MODEL /////////////////////////////

UInteractMLClassificationModelFactory::UInteractMLClassificationModelFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UInteractMLClassificationModel::StaticClass();
}

UObject* UInteractMLClassificationModelFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UInteractMLClassificationModel* NewObjectAsset = NewObject<UInteractMLClassificationModel>(InParent, Name, Flags | RF_Transactional);

	//ensure a guid is assigned
	NewObjectAsset->UpdateDerivedState();

	return NewObjectAsset;
}

////////////////////////////// REGRESSION MODEL /////////////////////////////

UInteractMLRegressionModelFactory::UInteractMLRegressionModelFactory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UInteractMLRegressionModel::StaticClass();
}

UObject* UInteractMLRegressionModelFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UInteractMLRegressionModel* NewObjectAsset = NewObject<UInteractMLRegressionModel>(InParent, Name, Flags | RF_Transactional);
	
	//ensure a guid is assigned
	NewObjectAsset->UpdateDerivedState();
	
	return NewObjectAsset;
}

////////////////////////////// DTW MODEL /////////////////////////////

UInteractMLDynamicTimewarpModelFactory::UInteractMLDynamicTimewarpModelFactory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UInteractMLDynamicTimeWarpModel::StaticClass();
}

UObject* UInteractMLDynamicTimewarpModelFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UInteractMLDynamicTimeWarpModel* NewObjectAsset = NewObject<UInteractMLDynamicTimeWarpModel>(InParent, Name, Flags | RF_Transactional);
	
	//ensure a guid is assigned
	NewObjectAsset->UpdateDerivedState();
	
	return NewObjectAsset;
}


#undef LOCTEXT_NAMESPACE

