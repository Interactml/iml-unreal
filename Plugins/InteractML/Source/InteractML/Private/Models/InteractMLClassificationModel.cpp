//----
// InteractML - University Of Arts London
//----

#include "InteractMLClassificationModel.h"

//unreal

//module

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for model data files
FString UInteractMLClassificationModel::cExtensionPrefix(TEXT(".classification"));

// LOCAL CLASSES & TYPES

// init on demand, clear, reset
//
void UInteractMLClassificationModel::ResetModelInstance()
{
	//create on demand
	if (!Model.IsValid())
	{
		Model = MakeShareable(new classificationFloat());
	}

	//reset
	Model->reset();
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

