//----
// InteractML - University Of Arts London
//----

#include "InteractMLTask.h"

//unreal

//module
#include "InteractML.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// LOCAL CLASSES & TYPES


//main operation (on background thread)
//
void FInteractMLTask::Run()
{
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

