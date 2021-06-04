//----
// InteractML - University Of Arts London
//----

#pragma once


//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLStorage.h"
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
	FString label;

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
UCLASS()
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
	
	// are we currently recording, and which node is doing it? (might be only true for a single frame if Single sample mode)
	FString RecordingNode;
	
	// are we currently resetting, and which node is doing it? (we track this so we can ignore continuous reset requests)
	FString ResettingNode;
	
	// accumulator for current active recording session
	FInteractMLExample CurrentRecording;
	
public:
	//---- constants ----
	
	// extension prefix for example/training data files
	static FString cExtensionPrefix;
	

	//---- access ----
	bool IsRecording() const { return !RecordingNode.IsEmpty(); }
	bool IsResetting() const { return !ResettingNode.IsEmpty(); }
	
	//---- modification ----
	bool BeginRecording( int label, FString node_id );
	bool RecordParameters( struct FInteractMLParameterCollection* parameters, FString node_id );
	bool EndRecording( FString node_id );

	void BeginReset( FString node_id );
	void EndReset( FString node_id );
	
	//each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const { return cExtensionPrefix; }

	// Begin UInteractMLStorage overrides
	virtual bool LoadJson(const FString& json_string) override;
	virtual bool SaveJson(FString& json_string) const override;
	// End UInteractMLStorage overrides
	

private:

	void ResetExamples();
	void ExtractCharacteristics();

};
