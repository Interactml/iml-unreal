//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once


//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLStorage.h"
#include "InteractMLHelpers.h"
#include "InteractMLTrainingSet.generated.h"

//general declarations



// single sample of training data parameters
//
USTRUCT()
struct INTERACTML_API FInteractMLSample
{
	GENERATED_BODY()
		
	UPROPERTY()
	TArray<float> inputData;
};

// training example that associates a label with a single or series of parameter samples
USTRUCT()
struct INTERACTML_API FInteractMLExample
{
	GENERATED_BODY()
		
	UPROPERTY()
	float label;

	UPROPERTY()
	TArray<FInteractMLSample> inputSeries;
};

// recording/storage/sampling mode, are samples a single parameter set or a series?
UENUM()
enum class EInteractMLSampleMode
{
	Unknown = 0,
	Single  = 1,
	Series  = 2
};


// InteractML Training Set
// represents collection of training data used to train a model
// holds in-memory version of training set, backed by underlying JSON file storage
//
UCLASS(BlueprintType)
class INTERACTML_API UInteractMLTrainingSet
	: public UInteractMLStorage
{
	GENERATED_BODY()
		
	//---- persistent state ----

	//the training data
	UPROPERTY()
	TArray<FInteractMLExample> Examples;
	

	//---- transient/cached state ----
		
	// how was our data recorded? (Unknown if not been set yet)
	EInteractMLSampleMode SampleMode;

	// how many parameters in each sample (0 if unknown)
	int ParameterCount;
	
	// accumulator for current active recording session
	FInteractMLExample CurrentRecording;
	
public:
	FNodeActionInterlock RecordingAction; // are we currently recording, and which node is doing it? (might be only true for a single frame if Single sample mode)
	FNodeActionInterlock ResettingAction; // are we currently resetting, and which node is doing it? (we track this so we can ignore continuous reset requests)
	

	//---- constants ----
	
	// extension prefix for example/training data files
	static FString cExtensionPrefix;
	

	//---- access ----
	bool HasExamples() const { return Examples.Num()>0; }
	const TArray<FInteractMLExample>& GetExamples() const { return Examples; }
	bool IsRecording() const { return RecordingAction.Active(); }
	bool IsResetting() const { return ResettingAction.Active(); }
	
	//---- modification ----
	bool BeginRecording( float label );
	bool RecordParameters( struct FInteractMLParameterCollection* parameters );
	bool EndRecording();

	void ResetTrainingSet();
	
	//each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const { return cExtensionPrefix; }

	// Begin UInteractMLStorage overrides
	virtual bool LoadJson(const FString& json_string) override;
	virtual bool SaveJson(FString& json_string) const override;
	// End UInteractMLStorage overrides
	
	//---- blueprint access ----
	//How many examples have been recorded into this training set
	UFUNCTION(BlueprintPure,Category="InteractML",meta=(CompactNodeTitle="Example Count"))
	int GetExampleCount();
	//How many parameters are there in each example (if known)
	UFUNCTION(BlueprintPure,Category="InteractML",meta=(CompactNodeTitle="Parameter Count"))
	int GetParameterCount();

	//---- static utility ----
	static bool LoadExamplesFromJson( const FString& json_string_in, TArray<FInteractMLExample>& examples_out );
	static bool SaveExamplesToJson( const TArray<FInteractMLExample>& examples_in, FString& json_string_out );
	static bool ClearExamplesCollection( TArray<FInteractMLExample>& examples );
	

private:

	void ResetExamples();
	void ExtractCharacteristics();

};
