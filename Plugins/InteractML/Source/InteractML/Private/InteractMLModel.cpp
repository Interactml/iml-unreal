//----
// InteractML - University Of Arts London
//----

#include "InteractMLModel.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLParameters.h"
#include "InteractMLTrainingSet.h"

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
bool UInteractMLModel::RunModel(FInteractMLParameterCollection* parameters, TArray<float>& out_values)
{
	return RunModelInstance( parameters, out_values );
}

// run the model against the provided series of parameter sets
// returns the values matched against during the run
// NOTE: for series match, runs synchronously, i.e. blocks until complete
//
bool UInteractMLModel::RunModel(FInteractMLParameterSeries* parameter_series, TArray<float>& out_values)
{
	return RunModelInstance( parameter_series, out_values );
}

// train the model with the provided training set
// NOTE: runs synchronously, i.e. blocks until complete
//
void UInteractMLModel::TrainModel(UInteractMLTrainingSet* training_set)
{
	//always reset before training
	ResetModel();

	//propagate label cache (needed for training)
	LabelCache.Assign( training_set->GetLabelCache() );

	//train
	bIsTrained = TrainModelInstance(training_set);
}

// reset the model to initialised but empty
//
void UInteractMLModel::ResetModel()
{
	ResetModelInstance();
	LabelCache.Reset();
	bIsTrained = false;
}


// fallback operation of running a single sample model, can be specialised
//
bool UInteractMLModel::RunModelInstance(struct FInteractMLParameterCollection* parameters, TArray<float>& out_values)
{
	check(!IsSeries()); //shouldn't be trying to run a series model with single input
	if (!IsTrained())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Running an untrained model: %s"), *GetFilePath());
		return false;
	}

	//convert parameter data to RapidLib form
	std::vector<float> model_inputs;
	for (int iparam = 0; iparam < parameters->Values.Num(); iparam++)
	{
		float value = parameters->Values[iparam];
		model_inputs.push_back(value);
	}
	
	//run the model
	modelSetFloat* model = GetModelInstance();
	std::vector<float> outputs;
	try
	{
		outputs = model->run( model_inputs );
	}
	catch(std::exception ex)
	{
		//handle?
		UE_LOG( LogInteractML, Error, TEXT( "Exception trying to run model %s : %s" ), *GetName(), StringCast<TCHAR>( ex.what() ).Get() );
		return false;
	}

	//expecting single label?
	int num_expected = IsDiscrete()?1:LabelCache.GetNumValues();
	bool success = outputs.size() == num_expected;

	//result
	if (success)
	{
		out_values.Reset();
		for (size_t i = 0; i < outputs.size(); i++)
		{
			out_values.Add(outputs[i]);
		}
		return true;
	}
	return false;
}

// fallback operation of training a model, can be specialised
//
bool UInteractMLModel::TrainModelInstance(UInteractMLTrainingSet* training_set)
{
	//gather
	const TArray<FInteractMLExample>& training_examples = training_set->GetExamples();

	//convert training data to RapidLib form
	//TODO: cache this for re-training without allocation
	std::vector<trainingExampleFloat> model_examples;
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
		if(IsContinuous() && training_set->HasCompositeLabels())
		{
			//composite label
			int label_index = (int)training_example.label;
			TArray<float> label_values;
			if(!LabelCache.GetLabel( label_index, label_values ))
			{
				UE_LOG( LogInteractML, Error, TEXT( "Failed to find label index %i in training set '%s' when training" ), label_index, *training_set->GetName() );
			}
			for(int i = 0; i<label_values.Num(); i++)
			{
				model_example.output.push_back( label_values[i] );
			}
		}
		else
		{
			//simple value OR label index
			model_example.output.push_back( training_example.label );
		}

		//add to set
		model_examples.push_back(model_example);
	}

	//train the model
	modelSetFloat* model = GetModelInstance();
	bool success = model->train(model_examples);

	//we have new state to save
	if (success)
	{
		MarkUnsavedData();
	}

	//result
	if(!success)
	{
		UE_LOG(LogInteractML, Error, TEXT("Training failed: %s"), *GetFilePath() );
	}
	return success;
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

