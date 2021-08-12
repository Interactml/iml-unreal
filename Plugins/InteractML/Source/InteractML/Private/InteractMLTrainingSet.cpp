//----
// InteractML - University Of Arts London
//----

#include "InteractMLTrainingSet.h"

//unreal
#include "JsonObjectConverter.h"

//module
#include "InteractML.h"
#include "InteractMLParameters.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for example/training data files
FString UInteractMLTrainingSet::cExtensionPrefix(TEXT(".training"));

// LOCAL CLASSES & TYPES

// recreate training set state from Json
//
bool UInteractMLTrainingSet::LoadJson(const FString& json_string)
{
	if (LoadExamplesFromJson(json_string, Examples))
	{
		//post-load analysis/fixups
		ValidateExamples();		
		ExtractCharacteristics();
			
		return true;		
	}
	
	//failed
	return false;
}

// turn training set state into Json
//
bool UInteractMLTrainingSet::SaveJson(FString& json_string) const
{
	return SaveExamplesToJson(Examples, json_string);
}

// empty out any example state
//
void UInteractMLTrainingSet::ResetExamples()
{
	ClearExamplesCollection(Examples);
	LabelCache.Reset();
	ParameterCount = 0;
}

// post load we can look at the data to determine:
// * is it single samples or series sampling?
// * how many parameters in the samples?
// * is it consistent? (i.e. all entries match)
//
void UInteractMLTrainingSet::ExtractCharacteristics()
{
	//reset
	SampleMode = EInteractMLSampleMode::Unknown;
	ParameterCount = 0;
	TSet<float> Labels;

	//scan examples
	int min_sample_count = MAX_int32;
	int max_sample_count = 0;
	int min_param_count = MAX_int32;
	int max_param_count = 0;
	for (int i = 0; i < Examples.Num(); i++)
	{
		FInteractMLExample& example = Examples[i];

		//example
		int series_size = example.inputSeries.Num();
		min_sample_count = FMath::Min(min_sample_count, series_size );
		max_sample_count = FMath::Max(max_sample_count, series_size );
		
		//samples
		for (int s = 0; s < example.inputSeries.Num(); s++)
		{
			FInteractMLSample& sample = example.inputSeries[s];

			//parameters
			int param_count = sample.inputData.Num();
			min_param_count = FMath::Min(min_param_count, param_count );
			max_param_count = FMath::Max(max_param_count, param_count );
		}

		//labels
		Labels.Add(example.label);
	}

	//distinct labels
	LabelCount = Labels.Num();

	//results
	bool invalid = false;
	//samples
	if (min_sample_count != MAX_int32)
	{
		if (min_sample_count==1 && max_sample_count==1)
		{
			//single sample examples
			SampleMode = EInteractMLSampleMode::Single;
		}
		else
		{
			//probably series (not definite, you could have recorded for one frame :/)
			//TODO: does series detection need to be more robust? (would need to store this information in the file, or associate it)
			SampleMode = EInteractMLSampleMode::Series;
		}
	}
	//parameters
	if (min_param_count != MAX_int32)
	{
		if (min_param_count != max_param_count)
		{
			//should all be the same
			invalid = true;
		}

		ParameterCount = min_param_count;
	}

	//report
	if (invalid)
	{
		UE_LOG(LogInteractML, Error, TEXT("Inconsistent parameter counts, ranging from %i to %i in training set '%s'"), min_param_count, max_param_count, *GetFilePath() );
	}
}

// validate data, apply any fixups/upgrades needed
//
void UInteractMLTrainingSet::ValidateExamples()
{
	//ensure ID's are assigned (upgrade old data)
	if (Examples.Num() >= 2) //(need at least 2 to check this)
	{
		if (Examples[0].ID == Examples[1].ID) //defaults
		{
			//assign them
			for (int i = 0; i < Examples.Num(); i++)
			{
				Examples[i].ID = i;
			}
		}
	}
}

// re-usable load function
//
bool UInteractMLTrainingSet::LoadExamplesFromJson(const FString& json_string_in, TArray<FInteractMLExample>& examples_out)
{
	//reset
	ClearExamplesCollection( examples_out );
	
	//load
	if (json_string_in.IsEmpty())
	{
		//no data
		return true;
	}
	
	//parse actual data
	if (FJsonObjectConverter::JsonArrayStringToUStruct( json_string_in, &examples_out, 0, 0 ))
	{
		return true;
	}
	
	//failed
	return false;
}

// re-usable save function
//
bool UInteractMLTrainingSet::SaveExamplesToJson(const TArray<FInteractMLExample>& examples_in, FString& json_string_out)
{
	//reset
	json_string_out = "";
	
	//have to manually convert array to json string it seems
	TArray<TSharedPtr<FJsonValue>> data_values;
	for(const FInteractMLExample& example : examples_in)
	{
		TSharedPtr<FJsonObject> example_object = FJsonObjectConverter::UStructToJsonObject( example );
		if(example_object.IsValid())
		{
			data_values.Add( MakeShared<FJsonValueObject>( example_object ) );
		}
	}
	TSharedRef<FJsonValueArray> ValuesArrayValue = MakeShared<FJsonValueArray>( data_values );
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create( &json_string_out );
	if (FJsonSerializer::Serialize(ValuesArrayValue, FString(), JsonWriter))
	{
		return true;
	}
	
	//failed
	return false;
}

// re-usable clear function
//
bool UInteractMLTrainingSet::ClearExamplesCollection(TArray<FInteractMLExample>& examples)
{
	examples.Empty();
	return true;
}


// check ready, prep and start recording - simple label
//
bool UInteractMLTrainingSet::BeginRecording(float label)
{
	//reset
	CurrentRecording.label = label;
	CurrentRecording.inputSeries.Empty();

	return true;
}

// check ready, prep and start recording - composite label
//
bool UInteractMLTrainingSet::BeginRecording(const UInteractMLLabel* label_type, const void* label_data)
{
	//associate this label expected output values with a numeric label for lookup later
	float label = LabelCache.Find(label_type, label_data);

	//reset
	CurrentRecording.label = label;
	CurrentRecording.inputSeries.Empty();
	
	return true;
}


// submit a parameter set to be accumulated with the current label
//
bool UInteractMLTrainingSet::RecordParameters(FInteractMLParameterCollection* parameters )
{
	int incoming_parameter_count = parameters->Values.Num();

	//checks
	check(IsRecording());
	if(ParameterCount!=0 && incoming_parameter_count!=ParameterCount)
	{
		UE_LOG(LogInteractML, Error, TEXT("Parameter set size has changed from %i to %i in training set '%s'. Please reset and re-record all examples."), ParameterCount, incoming_parameter_count, *GetFilePath() );
		return false;
	}

	//new sample
	int index = CurrentRecording.inputSeries.Emplace();
	FInteractMLSample& sample = CurrentRecording.inputSeries[index];

	//fill in parameters
	sample.inputData.SetNum(incoming_parameter_count);
	for (int i = 0; i < incoming_parameter_count; i++)
	{
		sample.inputData[i] = parameters->Values[i];
	}

	//update param count if first one
	if(ParameterCount==0)
	{
		ParameterCount = incoming_parameter_count;
	}

	return true;
}

// done recording, issue accumulated samples to example list and tidy up
//
bool UInteractMLTrainingSet::EndRecording()
{
	bool success = false;

	//any data added?
	if (CurrentRecording.inputSeries.Num() > 0)
	{
		//find id for it
		int max_id = -1;
		for (int i = 0; i < Examples.Num(); i++)
		{
			max_id = FMath::Max(max_id, Examples[i].ID);
		}
		CurrentRecording.ID = max_id + 1;

		//move finished recording into the example set
		Examples.Add( CurrentRecording );
		success = true;

		//flag as dirty
		MarkUnsavedData();

		//re-eval state to cache some stats/info
		ExtractCharacteristics();
	}
	
	//done
	return success;
}

// start reset (split because we are driven from a bool and not an event so we need to track on/off state change)
//
void UInteractMLTrainingSet::ResetTrainingSet()
{
	//perform reset
	UE_LOG(LogInteractML, Log, TEXT("Resetting training set '%s'"), *GetFilePath());
	ResetExamples();
	MarkUnsavedData();
}

//////////////////////// editing //////////////////////////////

// remove an example by ID
//
bool UInteractMLTrainingSet::RemoveExample(int example_id, FInteractMLExample* out_removed_example )
{
	//find example
	int example_index = -1;
	for (int i = 0; i < Examples.Num(); i++)
	{
		if (Examples[i].ID == example_id)
		{
			example_index = i;
			break;
		}
	}

	//remove
	if (example_index != -1)
	{
		//copy out
		if (out_removed_example)
		{
			*out_removed_example = Examples[example_index];
		}

		//remove
		Examples.RemoveAt(example_index);
		return true;
	}

	return false;
}




//////////////////////// blueprint access /////////////////////////

int UInteractMLTrainingSet::GetExampleCount()
{
	return Examples.Num();
}
int UInteractMLTrainingSet::GetParameterCount()
{
	return ParameterCount;
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

