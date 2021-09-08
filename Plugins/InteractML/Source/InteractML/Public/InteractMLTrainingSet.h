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
#include "InteractMLLabelCache.h"
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
		
	//each recorded example has an unique ID that is maintained and never repeated (within its training set)
	UPROPERTY()
	int ID=0;

	//the label this examples recorded parameters are to be associated with
	UPROPERTY()
	float label;

	//samples in this example (1 for single mode, potentially >1 for series)
	UPROPERTY()
	TArray<FInteractMLSample> inputSeries;

	//additional info

	//the timestamp of the recording session
	UPROPERTY()
	FString Session;

	//the name of the user that recorded this
	UPROPERTY()
	FString User;

	//duration of recording (for series)
	UPROPERTY()
	float Duration;
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
class INTERACTML_API UInteractMLTrainingSet	: public UInteractMLStorage
{
	GENERATED_BODY()
		
	//---- persistent state ----

	//the training data
	UPROPERTY()
	TArray<FInteractMLExample> Examples;

	//label information
	UPROPERTY()
	FInteractMLLabelCache LabelCache;

	//---- transient/cached state ----
		
	// how was our data recorded? (Unknown if not been set yet)
	EInteractMLSampleMode SampleMode;

	// how many parameters in each sample (0 if unknown)
	int ParameterCount;

	// how many distinct labels?
	int LabelCount;
	
	// accumulator for current active recording session
	FInteractMLExample CurrentRecording;

	// tracking duration of recording
	FDateTime RecordingStart;
	
public:
	FNodeActionInterlock RecordingAction; // are we currently recording, and which node is doing it? (might be only true for a single frame if Single sample mode)
	FNodeActionInterlock DeletingLastAction; // are we currently resetting (one), and which node is doing it? (we track this so we can ignore continuous reset requests)
	FNodeActionInterlock DeletingLabelAction; // are we currently resetting (some), and which node is doing it? (we track this so we can ignore continuous reset requests)
	FNodeActionInterlock DeletingAllAction; // are we currently resetting (all), and which node is doing it? (we track this so we can ignore continuous reset requests)
	
	//---- constants ----
	
	// extension prefix for example/training data files
	static FString cExtensionPrefix;

	//---- access ----
	bool HasBeenReset() const { return !HasExamples() && SampleMode==EInteractMLSampleMode::Unknown && ParameterCount==0 && LabelCount==0; }
	bool HasExamples() const { return Examples.Num()>0; }
	const TArray<FInteractMLExample>& GetExamples() const { return Examples; }
	bool IsSingleSamples() const { return SampleMode == EInteractMLSampleMode::Single; }
	bool IsSeriesSamples() const { return SampleMode == EInteractMLSampleMode::Series; }
	//BP (version below) int GetParameterCount() const { return ParameterCount; }
	int GetLabelCount() const { return LabelCount; }
	//interaction
	bool IsRecording() const { return RecordingAction.Active(); }
	bool IsDeletingLast() const { return DeletingLastAction.Active(); }
	bool IsDeletingLabel() const { return DeletingLabelAction.Active(); }
	bool IsDeletingAll() const { return DeletingAllAction.Active(); }
	
	const FInteractMLLabelCache& GetLabelCache() const { return LabelCache; }
	bool HasCompositeLabels() const { return LabelCache.LabelType != nullptr; }
	
	//---- modification ----
	bool BeginRecording( float label );
	bool BeginRecording( const UInteractMLLabel* label_type, const void* label_data );
	bool RecordParameters( struct FInteractMLParameterCollection* parameters );
	bool EndRecording();
	void DeleteLastExample();
	void DeleteLabelExamples( float label );
	void DeleteLabelExamples( const UInteractMLLabel* label_type, const void* label_data );
	void DeleteAllExamples();
	
	//---- editing ----
	bool RemoveExample( int example_id, FInteractMLExample* out_removed_example=nullptr );
	
	//each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const { return cExtensionPrefix; }

	// Begin UInteractMLStorage overrides
	virtual bool LoadJson(const FString& json_string) override;
	virtual bool SaveJson(FString& json_string) const override;
	// End UInteractMLStorage overrides
	
	// Begin UObject overrides
#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif
	// End UObject overrides
	
	//---- blueprint access ----
	//How many parameters are there in each example (if known)
	UFUNCTION(BlueprintPure,Category="InteractML",meta=(CompactNodeTitle="Parameter Count"))
	int GetParameterCount() const;
	//How many examples have been recorded into this training set
	UFUNCTION(BlueprintPure,Category="InteractML",meta=(CompactNodeTitle="Example Count"))
	int GetExampleCount() const;
	//How many examples have been recorded into this training set for a specific (simple) output
	UFUNCTION(BlueprintPure,Category="InteractML",meta=(CompactNodeTitle="Example Count (Simple Output)"))
	int GetExampleCountForSimpleOutput( float expected_output );
	//How many examples have been recorded into this training set for a specific (composite) output
	UFUNCTION(BlueprintPure,Category="InteractML",CustomThunk,meta=(CustomStructureParam="LabelData", CompactNodeTitle="Example Count (Composite Output)"))
	int GetExampleCountForCompositeOutput( const UInteractMLLabel* LabelType, const FGenericStruct& LabelData);
	//generic handler for any UInteractMLLabel struct type
	int Generic_GetExampleCountForCompositeOutput(const UInteractMLLabel* LabelType, const void* LabelData);
	/** Based on UInteractMLTrainingSet::execGetExampleCountForCompositeOutput */
	// const UInteractMLLabel* LabelType, 
	// const FGenericStruct& LabelData
	DECLARE_FUNCTION(execGetExampleCountForCompositeOutput)
	{
		P_GET_OBJECT(UInteractMLLabel, LabelType);
		Stack.StepCompiledIn<FStructProperty>(NULL); //P_GET_PROPERTY(FStructProperty, LabelData); - dont' need named prop entry, structs passed via pointer
		void* LabelDataStructAddr = Stack.MostRecentPropertyAddress;
		P_FINISH;
		
		int Return = 0;
		P_NATIVE_BEGIN;
		Return = P_THIS->Generic_GetExampleCountForCompositeOutput(LabelType, LabelDataStructAddr);
		P_NATIVE_END;	
		*(int*)RESULT_PARAM = Return;
	}

	//---- static utility ----
	static bool LoadExamplesFromJson( const FString& json_string_in, TArray<FInteractMLExample>& examples_out );
	static bool SaveExamplesToJson( const TArray<FInteractMLExample>& examples_in, FString& json_string_out );
	static bool ClearExamplesCollection( TArray<FInteractMLExample>& examples );
	
private:

	void ResetExamples();
	void ValidateExamples();
	void RefreshDerivedState();
	void ExtractCharacteristics();
	void NewRecordingSession( float label_number );

};
