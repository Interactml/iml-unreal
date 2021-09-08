//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLHelpers.h"
#include "InteractMLParameters.h"
#include "InteractMLTask.h"

//general declarations

// InteractML per-node state for running models (e.g. separate accumulation of series)
// sometimes needed so that multiple nodes can run the same model independantly
// obtained/cached by UInteractMLContext object in the nodes acto
//
class INTERACTML_API FInteractMLModelState : public TSharedFromThis<FInteractMLModelState>
{
public:		
	// node currently running this model
	FNodeActionInterlock RunAction;
	
	// parameter sample accumulation
	FInteractMLParameterSeries ParameterSeries;

	// currently running the model asynchronously
	FInteractMLTask::Ptr Task;

	// completion flag (true until checked)
	mutable bool bCompleted;

	// last result calculated by running the model
	TArray<float> CurrentResult;

	// reset to initial state
	void Reset();

	// async handling : a run is starting
	void StartRunning(FInteractMLTask::Ptr run_task);

	// async handling : a run has stopped
	void StopRunning(FInteractMLTask::Ptr run_task);

	// currently running a model (async)
	bool IsRunning() const { return Task.IsValid(); }

	// completed running a model?
	bool CheckCompleted() const { bool completed = bCompleted; bCompleted = false; return completed; }
};
