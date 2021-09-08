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
#include "InteractMLTrainingSetFactory.generated.h"


// makes a training set asset
//
UCLASS()
class INTERACTMLEDITOR_API UInteractMLTrainingSetFactory
	: public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags tFlags, UObject* Context, FFeedbackContext* Warn) override;
};

