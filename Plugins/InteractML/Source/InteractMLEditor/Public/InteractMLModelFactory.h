//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

// unreal
#include "Editor.h"

// apparance

// module

// auto (last)
#include "InteractMLModelFactory.generated.h"


////////////////////////////// CLASSIFICATION MODEL /////////////////////////////

// makes a classification model asset
//
UCLASS()
class INTERACTMLEDITOR_API UInteractMLClassificationModelFactory
	: public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags tFlags, UObject* Context, FFeedbackContext* Warn) override;
};


////////////////////////////// REGRESSION MODEL /////////////////////////////

// makes a regression model asset
//
UCLASS()
class INTERACTMLEDITOR_API UInteractMLRegressionModelFactory
	: public UFactory
{
	GENERATED_UCLASS_BODY()
		
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags tFlags, UObject* Context, FFeedbackContext* Warn) override;
};


////////////////////////////// DTW MODEL /////////////////////////////

// makes a DTW model asset
//
UCLASS()
class INTERACTMLEDITOR_API UInteractMLDynamicTimewarpModelFactory
	: public UFactory
{
	GENERATED_UCLASS_BODY()
		
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags tFlags, UObject* Context, FFeedbackContext* Warn) override;
};

