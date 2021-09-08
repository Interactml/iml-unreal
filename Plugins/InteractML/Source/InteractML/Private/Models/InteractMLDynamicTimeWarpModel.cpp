//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "Models/InteractMLDynamicTimeWarpModel.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLParameters.h"
#include "InteractMLTask.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for model data files
FString UInteractMLDynamicTimeWarpModel::cExtensionPrefix(TEXT(".dtw"));

// LOCAL CLASSES & TYPES

// prepare for DTW running
//
FInteractMLTask::Ptr UInteractMLDynamicTimeWarpModel::BeginRunningModel(struct FInteractMLParameterSeries* parameter_series)
{
	//check
	check(IsSeries()); //shouldn't be trying to run a series model with single input
	if (!IsTrained())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Running an untrained model: %s"), *GetFilePath());
		return nullptr;
	}

	//create running task
	FInteractMLTask::Ptr task = MakeShareable(new FInteractMLTask(this, EInteractMLTaskType::Run));

	//convert data to RapidLib form
	int num_samples = parameter_series->Num();
	for (int isamples = 0; isamples < num_samples; isamples++)
	{
		//emplace new collection for snapshot values (avoid copy after populating)
		task->InputSeries.push_back(std::vector<float>());
		std::vector<float>& model_input_parameters = task->InputSeries.back();

		//incoming snapshot
		const FInteractMLParameterCollection* parameters = parameter_series->GetSample(isamples);

		//fill them in
		int num_values = parameters->Values.Num();
		model_input_parameters.reserve(num_values); //only realloc once
		for (int i = 0; i < num_values; i++)
		{
			float value = parameters->Values[i];
			model_input_parameters.push_back(value);
		}
	}

	return task;
}

// run the DTW model
// NOTE: Multi-threaded call, must be handled thread safely, only for direct training/running using task state
//
void UInteractMLDynamicTimeWarpModel::DoRunningModel( FInteractMLTask::Ptr run_task )
{	
	//don't run on 0 samples (used to crash the model)
	if(run_task->InputSeries.size() > 0)
	{
		//with the accumulated input parameter series we can now run the model
		std::string label_text;
#if INTERACTML_TRAP_CPP_EXCEPTIONS
		try
#endif
		{
			label_text = Model->run( run_task->InputSeries );
		}
#if INTERACTML_TRAP_CPP_EXCEPTIONS
		catch(std::exception ex)
		{
			//handle?
			UE_LOG( LogInteractML, Error, TEXT( "Exception trying to run DTW model %s : %s" ), *GetName(), StringCast<TCHAR>( ex.what() ).Get() );
		}
#endif
		
		//interpret label
		bool success = label_text.size() > 0;
		if(success)
		{
			//interpret result
			float label = FCStringAnsi::Atof( label_text.c_str() );
			
			run_task->Outputs.Reset();
			run_task->Outputs.Add(label);
			run_task->bSuccess = true;
		}
	}
	
	run_task->bSuccess = true;
}

// preparation for training dtw model
// NOTE: not async training, as it's just a copy, saves processing/passing state to task object
//
FInteractMLTask::Ptr UInteractMLDynamicTimeWarpModel::BeginTrainingModel(UInteractMLTrainingSet* training_set)
{
	//create training task
	FInteractMLTask::Ptr task = MakeShareable( new FInteractMLTask( this, EInteractMLTaskType::Train ) );

	//we keep a copy of our training set
	Examples = training_set->GetExamples();

	//gather and load new training state into model
	task->bSuccess = ApplyExamples();	
	
	return task;
}

// nothing to do for DTW training
// NOTE: Multithreaded call, only train in context of the training task state or known thread-safe calls
//
void UInteractMLDynamicTimeWarpModel::DoTrainingModel( FInteractMLTask::Ptr training_task )
{
	//nothing to do, training happens in Begin (since it's just a simple copy op)
}

// init on demand, clear, reset
//
void UInteractMLDynamicTimeWarpModel::ResetModelInstance()
{
	//create on demand
	if (!Model.IsValid())
	{
		Model = MakeShareable(new seriesClassificationFloat());
	}

	//reset
	Model->reset();

	//reset training state
	UInteractMLTrainingSet::ClearExamplesCollection( Examples );
}

// transfer current example data into model to allow running of the model
//
bool UInteractMLDynamicTimeWarpModel::ApplyExamples()
{
	const TArray<FInteractMLExample>& training_examples = Examples;
	
	//convert training data to RapidLib form
	//TODO: cache this for re-training without allocation
	std::vector<trainingSeriesFloat> model_examples;
	for (int iexample = 0; iexample < training_examples.Num(); iexample++)
	{
		const FInteractMLExample& training_example = training_examples[iexample];
		
		//series
		trainingSeriesFloat model_example;
		for (int iseries = 0; iseries < training_example.inputSeries.Num(); iseries++)
		{
			//data values for this set
			const FInteractMLSample& training_data = training_example.inputSeries[iseries];
			
			//add parameter array
			model_example.input.push_back(std::vector<float>());
			std::vector<float>& model_data = model_example.input.back();
			
			//fill in
			for (int ivalue = 0; ivalue < training_data.inputData.Num(); ivalue++)
			{
				float value = training_data.inputData[ivalue];
				model_data.push_back( value );
			}
		}
		
		//label
		FString label_text = FString::Format(TEXT("{0}"), { training_example.label });
		model_example.label.assign( StringCast<ANSICHAR>(*label_text).Get() );
		
		//add to set
		model_examples.push_back( model_example );
	}
	
	//when we say "train" the model, for dtw this is just loading in the example data
	Model->reset();
	bool success = true;
	if(model_examples.size() > 0)
	{
		success = Model->train( model_examples );
	}
	
	return success;
}

// read and restore stored model state into actual RapidLib model
//
bool UInteractMLDynamicTimeWarpModel::LoadJson(const FString& json_string)
{
	ResetModel();

	//trained state is stored as training set
	bool ok = UInteractMLTrainingSet::LoadExamplesFromJson( json_string, Examples );
	
	//gather and load new training state into model when it is applied
	bIsTrained = ApplyExamples();
	
	return ok;
}

// save out RapidLib model state
//
bool UInteractMLDynamicTimeWarpModel::SaveJson(FString& json_string) const
{
	//trained state is stored as training set
	bool ok = UInteractMLTrainingSet::SaveExamplesToJson( Examples, json_string );
	
	return ok;
}

// EPILOGUE
#undef LOCTEXT_NAMESPACE

