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
	//reset
	ResetExamples();

	//load
	if (json_string.IsEmpty())
	{
		//no data
		return true;
	}

	//parse actual data
	if (FJsonObjectConverter::JsonArrayStringToUStruct( json_string, &Examples, 0, 0 ))
	{
		//post-load analysis
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
	//reset
	json_string = "";
	
	//have to manually convert array to json string it seems
	TArray<TSharedPtr<FJsonValue>> data_values;
	for(const FInteractMLExample& example : Examples)
	{
		TSharedPtr<FJsonObject> example_object = FJsonObjectConverter::UStructToJsonObject( example );
		if(example_object.IsValid())
		{
			data_values.Add( MakeShared<FJsonValueObject>( example_object ) );
		}
	}
	TSharedRef<FJsonValueArray> ValuesArrayValue = MakeShared<FJsonValueArray>( data_values );
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create( &json_string );
	if (FJsonSerializer::Serialize(ValuesArrayValue, FString(), JsonWriter))
	{
		return true;
	}

	//failed
	return false;
}

// empty out any example state
//
void UInteractMLTrainingSet::ResetExamples()
{
	Examples.Empty();
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
	}

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

// check ready, prep and start recording
//
bool UInteractMLTrainingSet::BeginRecording(int label)
{
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
		//move finished recording into the example set
		Examples.Add( CurrentRecording );
		success = true;

		//flag as dirty
		MarkUnsavedData();

		//can we determine sample size?
		if (SampleMode == EInteractMLSampleMode::Unknown)
		{
			//TODO: does series detection need to be more robust? (would need to explicitly pass in the mode)
			if (CurrentRecording.inputSeries.Num() == 1)
			{
				SampleMode = EInteractMLSampleMode::Single;
			}
			else
			{
				SampleMode = EInteractMLSampleMode::Series;
			}
		}
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

