//----
// InteractML - University Of Arts London
//----

#include "InteractMLDynamicTimeWarpModel.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLTrainingSet.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for model data files
FString UInteractMLDynamicTimeWarpModel::cExtensionPrefix(TEXT(".dtw"));

// LOCAL CLASSES & TYPES

// handle DTW running
//
float UInteractMLDynamicTimeWarpModel::RunModelInstance(struct FInteractMLParameterCollection* parameters)
{
	if (!IsTrained())
	{
		UE_LOG(LogInteractML, Warning, TEXT("Running an untrained model: %s"), *GetFilePath());
		return 0;
	}
	
#if 0
	//convert parameter data to RapidLib form
	std::vector<float> model_inputs;
	for (int iparam = 0; iparam < parameters->Values.Num(); iparam++)
	{
		float value = parameters->Values[iparam];
		model_inputs.push_back(value);
	}
	
	//run the model
	modelSetFloat* model = GetModelInstance();
	std::vector<float> outputs = model->run(model_inputs);
	
	//expecting single label
	bool success = outputs.size() == 1;
	
	return success?outputs[0]:0.0f;
#else
	return 0.0f;
#endif
}

// handle DTW training
//
bool UInteractMLDynamicTimeWarpModel::TrainModelInstance(class UInteractMLTrainingSet* training_set)
{
	//gather
	const TArray<FInteractMLExample>& training_examples = training_set->GetExamples();
	
#if 0
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
		
		//output (only one label used)
		float label_output = (float)training_example.label;
		model_example.output.push_back( label_output );
		
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
#else
	bool success = false;
#endif

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
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

