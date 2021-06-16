//----
// InteractML - University Of Arts London
//----

#include "InteractMLDynamicTimeWarpModel.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLParameters.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for model data files
FString UInteractMLDynamicTimeWarpModel::cExtensionPrefix(TEXT(".dtw"));

// LOCAL CLASSES & TYPES

// handle DTW running
//
float UInteractMLDynamicTimeWarpModel::RunModelInstance(struct FInteractMLParameterSeries* parameter_series)
{
	//check
	if (!IsTrained())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Running an untrained model: %s"), *GetFilePath());
		return false;
	}

	//co

	//convert to RapidLib form
	std::vector<std::vector<float>> InputParameters;
	int num_samples = parameter_series->Num();
	for (int isamples = 0; isamples < num_samples; isamples++)
	{
		//emplace new collection for snapshot values (avoid copy after populating)
		InputParameters.push_back(std::vector<float>());
		std::vector<float>& model_input_parameters = InputParameters.back();

		//incoming snapshot
		const FInteractMLParameterCollection* parameters = parameter_series->GetSample( isamples );

		//fill them in
		int num_values = parameters->Values.Num();
		model_input_parameters.reserve(num_values); //only realloc once
		for (int i = 0; i < num_values; i++)
		{
			float value = parameters->Values[i];
			model_input_parameters.push_back(value);
		}
	}

	//with the accumulated input parameter series we can now run the model
	std::string label_text = Model->run( InputParameters );

	//interpret label
	bool success = label_text.size()>0;
	if (success)
	{
		//interpret result
		float label = FCStringAnsi::Atof(label_text.c_str());
		return label;
	}

	return 0.0f;
}

// handle DTW training
//
bool UInteractMLDynamicTimeWarpModel::TrainModelInstance(class UInteractMLTrainingSet* training_set)
{
	//we keep a copy of our training set
	Examples = training_set->GetExamples();
	
	//gather and load new training state into model when it is loaded from storage
	bool success = ApplyExamples();
	bIsTrained = success;
	
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
	bool success = Model->train(model_examples);
	
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

