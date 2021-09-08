//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLTask.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLModel.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// LOCAL CLASSES & TYPES



//main operation (on background thread)
//
void FInteractMLTask::Run()
{
	//dispatch based on task type
	switch (Type)
	{
		case EInteractMLTaskType::Train:
			Model->DoTrainingModel( AsShared() );
			break;
		case EInteractMLTaskType::Run:
			Model->DoRunningModel( AsShared() );
			break;
		default:
			break;
	}
}

//apply results (on main thread)
void FInteractMLTask::Apply()
{
	//dispatch based on task type
	switch (Type)
	{
		case EInteractMLTaskType::Train:
			Model->EndTrainingModel( AsShared() );
			break;
		case EInteractMLTaskType::Run:
			Model->EndRunningModel( AsShared() );
			break;
		default:
			break;
	}
}




// EPILOGUE
#undef LOCTEXT_NAMESPACE

