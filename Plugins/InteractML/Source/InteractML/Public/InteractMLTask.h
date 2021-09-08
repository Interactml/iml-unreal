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

//rapidlib
#include "rapidLib.h"
using namespace rapidLib;
using modelSetFloat = modelSet<float>;

//general declarations


//task type
enum class EInteractMLTaskType
{
	None,
	Train,
	Run,
};


// InteractML Task
// Base for encapsulation of training or running a model, and any state they may need
// Can be dispatched off to other threads to run asynchronously if needed
// Monolithic (rather then inheritance hierarchy) to make handling of results simpler
//
struct FInteractMLTask : public TSharedFromThis<FInteractMLTask,ESPMode::ThreadSafe>
{
	//pointers to tasks need to be thread safe, use this typedef
	typedef TSharedPtr<FInteractMLTask, ESPMode::ThreadSafe> Ptr;

	//////////////// INPUT //////////////

	//type of operation
	EInteractMLTaskType Type;

	//model to operate on
	class UInteractMLModel* Model;

	//model state that running operates in the context of
	TSharedPtr<class FInteractMLModelState> Context;

	//training input
	std::vector<trainingExampleFloat> Examples;

	//run inputs (single sampling)
	std::vector<float> Inputs;

	//run inputs (series sampling)
	std::vector<std::vector<float>> InputSeries;

	//////////////// OUTPUT //////////////

	//flag as cancelled
	volatile bool bCancelled;

	//successful operation?
	bool bSuccess;

	//run output values
	TArray<float> Outputs;	

	//////////////// SETUP //////////////

	FInteractMLTask(UInteractMLModel* model, EInteractMLTaskType action)
		: Type(action)
		, Model(model)
		, Context(nullptr)
		, bCancelled( false )
		, bSuccess(false)
	{
	}

	//////////////// EXECUTION //////////////
	
	//main operation (on background thread)
	void Run();

	//apply results (on main thread)
	void Apply();

	//mark for cancel
	void Cancel() { bCancelled = true; }

};
