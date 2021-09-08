//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "Models/InteractMLRegressionModel.h"

//unreal

//module

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for model data files
FString UInteractMLRegressionModel::cExtensionPrefix(TEXT(".regression"));

// LOCAL CLASSES & TYPES

// init on demand, clear, reset
//
void UInteractMLRegressionModel::ResetModelInstance()
{
	//create on demand
	if (!Model.IsValid())
	{
		Model = MakeShareable(new regressionFloat());
	}

	//reset
	Model->reset();
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

