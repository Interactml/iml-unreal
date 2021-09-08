//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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
void FInteractMLModelState::StartRunning(FInteractMLTask::Ptr run_task)
{
	//check as run can be blocked sometimes and we don't get a task (e.g. prevented from running concurrently)
	if(run_task.IsValid())
	{
		//set context so model knows who to tell when run completed
		run_task->Context = AsShared();

		//remember our active task (indicates we are currently running)
		Task = run_task;

		//dispatch to actually start the run
		FInteractMLModule::Get().RunTask( run_task );
	}
}

// async handling : a run has stopped
//
void FInteractMLModelState::StopRunning(FInteractMLTask::Ptr run_task)
{
	check( run_task );

	//take on results
	CurrentResult = run_task->Outputs;

	//flag completion
	bCompleted = true;

	//reset state, release completed task
	Task.Reset();
}



// EPILOGUE
#undef LOCTEXT_NAMESPACE

