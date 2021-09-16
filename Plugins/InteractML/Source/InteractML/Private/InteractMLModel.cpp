//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLModel.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLParameters.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLTask.h"
#include "InteractMLModelState.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for model data files
FString UInteractMLModel::cExtensionPrefix(TEXT(".model"));

// LOCAL CLASSES & TYPES

// nothing to load, created a new one
//
void UInteractMLModel::Create()
{
	//ensure initialised
	ResetModel();
}

// read and restore stored model state into actual RapidLib model
//
bool UInteractMLModel::LoadJson( const FString& json_string )
{
	//ensure model initialised and clear
	ResetModel();

	//set up model from json
	modelSetFloat* model = GetModelInstance();

	//anything to do?
	bool ok = true;
	if(model && !json_string.IsEmpty())
	{
		//unfortunately we need to convert FString to std::string for this
		auto ansi_json = StringCast<ANSICHAR>( *json_string );
		std::string std_json = ansi_json.Get();

		//apply
		ok = model->putJSON( std_json );

		//TODO: shouldn't assume that a loaded model is trained, currently can't tell
		bIsTrained = true;
	}
	return ok;
}

// save out RapidLib model state
//
bool UInteractMLModel::SaveJson(FString& json_string) const
{
	//extract json
	modelSetFloat* model = GetModelInstance();
	if(model)
	{
		std::string std_json = model->getJSON();

		//unfortunately we need to convert the std::string back to an FString for this
		auto tchar_json = StringCast<TCHAR>( std_json.c_str() );
		json_string = tchar_json.Get();
	}
	else
	{
		//nothing to save
		json_string.Empty();
	}

	return true;
}

// run the model against the single provided parameter set
// returns the values matched against during the run
// NOTE: for single match, runs synchronously, i.e. blocks until complete
//
bool UInteractMLModel::RunModel(FInteractMLParameterCollection* parameters, TArray<float>& outputs)
{
	FInteractMLTask::Ptr task = BeginRunningModel( parameters );
	if(task.IsValid())
	{
		//train now, blocking execution until complete
		//UE_LOG( LogInteractML, Display, TEXT( "Running model '%s' synchronously" ), *GetName() );
		task->Run();
		task->Apply();
		//results
		outputs = task->Outputs;
		return task->bSuccess;
	}
	else
	{
		outputs.Empty();
		return false;
	}
}

// run the model against the provided series of parameter sets
// returns the values matched against during the run
// NOTE: for series match, runs synchronously, i.e. blocks until complete
//
bool UInteractMLModel::RunModel(FInteractMLParameterSeries* parameter_series, TArray<float>& outputs )
{
	FInteractMLTask::Ptr task = BeginRunningModel( parameter_series );
	if (task.IsValid())
	{
		//train now, blocking execution until complete
		//UE_LOG( LogInteractML, Display, TEXT( "Running model '%s' synchronously" ), *GetName() );
		task->Run();
		task->Apply();
		//results
		outputs = task->Outputs;
		return task->bSuccess;
	}
	else
	{
		outputs.Empty();
		return false;
	}
}

// run the model against the single provided parameter set
// NOTE: for single match, runs asynchronously, i.e. no results until task completes
//
FInteractMLTask::Ptr UInteractMLModel::RunModelAsync(FInteractMLParameterCollection* parameters)
{
	FInteractMLTask::Ptr task = BeginRunningModel(parameters);
	if (task.IsValid())
	{
		if (CheckAddTask(task))
		{
			return task;
		}
	}
	else
	{
		return nullptr;
	}

	//can't run concurrently
	UE_LOG(LogInteractML, Error, TEXT("Unable to run model %s concurrently because the RapidLib library doesn't support it yet for %s."), *GetName(), *GetClass()->GetName() );
	return nullptr;
}

// run the model against the provided series of parameter sets
// NOTE: for series match, runs asynchronously, i.e. no results until task completes
//
FInteractMLTask::Ptr UInteractMLModel::RunModelAsync(FInteractMLParameterSeries* parameter_series)
{
	FInteractMLTask::Ptr task = BeginRunningModel(parameter_series);
	if (task.IsValid())
	{
		if (CheckAddTask(task))
		{
			return task;
		}
	}
	else
	{
		return nullptr;
	}

	//can't run concurrently
	UE_LOG(LogInteractML, Error, TEXT("Unable to run model %s concurrently because the RapidLib library doesn't support it yet for %s."), *GetName(), *GetClass()->GetName() );
	return nullptr;
}

// train the model with the provided training set
// NOTE: runs synchronously, i.e. blocks until complete
//
void UInteractMLModel::TrainModel(UInteractMLTrainingSet* training_set)
{
	if(IsTraining())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Attempt to train model '%s' synchronously when already being trained asychronously"), *GetName());
		return;
	}
	
	//always reset before training
	ResetModel();

	//propagate label cache (needed for training)
	LabelCache.Assign( training_set->GetLabelCache() );

	//train now, blocking execution until complete
	TrainingTask = BeginTrainingModel( training_set );
	//UE_LOG( LogInteractML, Display, TEXT( "Training model '%s' synchronously" ), *GetName() );
	TrainingTask->Run();
	TrainingTask->Apply();
}

// train the model with the provided training set
// NOTE: runs asynchronously, i.e. non-blocking deferred completion/notification
//
void UInteractMLModel::TrainModelAsync(UInteractMLTrainingSet* training_set)
{
	if(IsTraining())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Attempt to train model '%s' concurrently when already being trained"), *GetName());
		return;
	}

	//always reset before training
	ResetModel();
	
	//propagate label cache (needed for training)
	LabelCache.Assign( training_set->GetLabelCache() );
	
	//train on another thread to run asynchronously	
	TrainingTask = BeginTrainingModel( training_set );
	//UE_LOG( LogInteractML, Display, TEXT( "Training model '%s' ASYNChronously" ), *GetName() );
	FInteractMLModule::Get().RunTask( TrainingTask );
}

// reset the model to initialised but empty
//
void UInteractMLModel::ResetModel()
{
	if(IsTraining())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Attempt to reset model '%s' whilst being trained asynchronously"), *GetName());
		return;
	}
	
	ResetModelInstance();
	bIsTrained = false;
}

// fallback preparation for training a model, can be specialised
//
FInteractMLTask::Ptr UInteractMLModel::BeginTrainingModel(UInteractMLTrainingSet* training_set)
{
	//create training task
	FInteractMLTask::Ptr task = MakeShareable( new FInteractMLTask( this, EInteractMLTaskType::Train ) );

	//gather
	const TArray<FInteractMLExample>& training_examples = training_set->GetExamples();

	//convert training data to RapidLib form
	//TODO: cache this for re-training without allocation
	for (int iexample = 0; iexample < training_examples.Num(); iexample++)
	{
		const FInteractMLExample& training_example = training_examples[iexample];

		trainingExampleFloat model_example;

		//inputs (single set)
		if (training_example.inputSeries.Num() == 1)
		{
			//data values for this set
			const FInteractMLSample& training_data = training_example.inputSeries[0];
			for (int ivalue = 0; ivalue < training_data.inputData.Num(); ivalue++)
			{
				float value = training_data.inputData[ivalue];
				model_example.input.push_back(value);
			}
		}

		//output look up label values
		//continuous label value outputs need to train on composite value list instead of indices
		//(assuming that composite values are available)
		if (IsContinuous() && training_set->HasCompositeLabels())
		{
			//composite label
			int label_index = (int)training_example.label;
			TArray<float> label_values;
			if (!LabelCache.GetLabel(label_index, label_values))
			{
				UE_LOG(LogInteractML, Error, TEXT("Failed to find label index %i in training set '%s' when training"), label_index, *training_set->GetName());
			}
			for (int i = 0; i < label_values.Num(); i++)
			{
				model_example.output.push_back(label_values[i]);
			}
		}
		else
		{
			//simple value OR label index
			model_example.output.push_back(training_example.label);
		}

		//add to set
		task->Examples.push_back(model_example);

		//ensure model will be available during execution
		check(GetModelInstance());
	}

	return task;
}

// do actual training of the model
// NOTE: Multithreaded call, only train in context of the training task state or known thread-safe calls
//
void UInteractMLModel::DoTrainingModel( FInteractMLTask::Ptr training_task )
{
	//train the model
	modelSetFloat* model = GetModelInstance();
#if INTERACTML_TRAP_CPP_EXCEPTIONS
	try
#endif
	{
		training_task->bSuccess = model->train( training_task->Examples );
	}
#if INTERACTML_TRAP_CPP_EXCEPTIONS
	catch(std::exception ex)
	{
		//handle?
		UE_LOG( LogInteractML, Error, TEXT( "Exception trying to train model %s : %s" ), *GetName(), StringCast<TCHAR>( ex.what() ).Get() );
	}
#endif
}

// handle results of training
void UInteractMLModel::EndTrainingModel( FInteractMLTask::Ptr training_task )
{
	//done
	check(training_task == TrainingTask);
	bIsTrained = training_task->bSuccess;
	bTrainingCompleted = true;
	TrainingTask.Reset();
	
	//we have new state to save
	if (bIsTrained)
	{
		MarkUnsavedData();
	}
	
	//result
	if(!bIsTrained)
	{
		UE_LOG(LogInteractML, Error, TEXT("Training failed: %s"), *GetFilePath() );
	}
}

// fallback operation of running a single sample model, can be specialised
//
FInteractMLTask::Ptr UInteractMLModel::BeginRunningModel(struct FInteractMLParameterCollection* parameters)
{
	check(!IsSeries()); //shouldn't be trying to run a series model with single input
	if (!IsTrained())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Running an untrained model: %s"), *GetFilePath());
		return nullptr;
	}

	//create running task
	FInteractMLTask::Ptr task = MakeShareable(new FInteractMLTask(this, EInteractMLTaskType::Run));

	//convert parameter data to RapidLib form
	for (int iparam = 0; iparam < parameters->Values.Num(); iparam++)
	{
		float value = parameters->Values[iparam];
		task->Inputs.push_back(value);
	}

	//ensure model will be available during execution
	check(GetModelInstance());

	return task;
}

// fallback operation of running a single sample model, can be specialised
// NOTE: Multi-threaded call, must be handled thread safely, only for direct training/running using task state
//
void UInteractMLModel::DoRunningModel(FInteractMLTask::Ptr run_task)
{
	//run the model
	modelSetFloat* model = GetModelInstance();
	check(model);
#if INTERACTML_TRAP_CPP_EXCEPTIONS
	try
#endif
	{
		//run
		std::vector<float> outputs;
		outputs = model->run(run_task->Inputs);

		//transfer out
		run_task->Outputs.Reset(outputs.size());
		for (size_t i = 0; i < outputs.size(); i++)
		{
			run_task->Outputs.Add(outputs[i]);
		}
	}
#if INTERACTML_TRAP_CPP_EXCEPTIONS
	catch(std::exception ex)
	{
		//handle?
		UE_LOG(LogInteractML, Error, TEXT("Exception trying to run model %s : %s"), *GetName(), StringCast<TCHAR>(ex.what()).Get());
	}
#endif
}

// handle results of running model
// NOTE: currently the same for all models, not usually overridden
//
void UInteractMLModel::EndRunningModel( FInteractMLTask::Ptr run_task )
{
	//expecting single label?
	int num_expected = IsDiscrete()?1:LabelCache.GetNumValues();
	run_task->bSuccess = run_task->Outputs.Num()==num_expected;

	//dispatch completed task to node context it started on
	//places results into context for lib fn to read
	if(run_task->Context) //(not used for sync operation)
	{
		run_task->Context->StopRunning(run_task);
	}

	//done running this model
	CheckRemoveTask(run_task);
}

// track model use: record this task as currently running this model
// returns true if you are allowed to run this task
//
bool UInteractMLModel::CheckAddTask(FInteractMLTask::Ptr task)
{
	//check concurency allowed
	if (RunTasks.Num() > 0			//(already running?)
		&& !CanRunConcurrently())	//(but not allowed!)
	{
		return false;
	}
	
	RunTasks.AddUnique(task);
	return true;
}

// track model use: clear record of this task running this model
//
void UInteractMLModel::CheckRemoveTask(FInteractMLTask::Ptr task)
{
	RunTasks.Remove(task);
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE
