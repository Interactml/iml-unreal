//----
// InteractML - University Of Arts London
//----

#include "InteractMLModelState.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLTask.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES

// async handling : a run is starting
//
void FInteractMLModelState::StartRunning(TSharedPtr<FInteractMLTask> run_task)
{
	//set context so model knows who to tell when run completed
	run_task->Context = this;

	//remember our active task (indicates we are currently running)
	Task = run_task;

	//dispatch to actually start the run
	FInteractMLModule::Get().RunTask( run_task );
}

// async handling : a run has stopped
//
void FInteractMLModelState::StopRunning(TSharedPtr<FInteractMLTask> run_task)
{
	//take on results
	CurrentResult = run_task->Outputs;

	//flag completion
	bCompleted = true;

	//reset state, release completed task
	Task.Reset();
}



// EPILOGUE
#undef LOCTEXT_NAMESPACE

