//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//reactml
#include "seriesClassification.h"

//module
#include "InteractMLModel.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLDynamicTimeWarpModel.generated.h"

//general declarations

// InteractML Model for running a Dynamic Time-warp algorithm
//
UCLASS(BlueprintType)
class INTERACTML_API UInteractMLDynamicTimeWarpModel
	: public UInteractMLModel
{
	GENERATED_BODY()	
		
	//---- persistent state ----
	//the trained state (for DTW it is the same as the training data so we just persist a copy)
	UPROPERTY()
	TArray<FInteractMLExample> Examples;
	
	//---- transient/cached state ----
	TSharedPtr<seriesClassificationFloat, ESPMode::ThreadSafe> Model;

public:
	//---- constants ----
	
	// extension prefix for model data files
	static FString cExtensionPrefix;
	
	//---- persistence ----
	virtual bool LoadJson(const FString& json_string) override;
	virtual bool SaveJson(FString& json_string) const override;

	//---- access ----
	
	//model type	
	virtual bool IsSeries() const { return true; }	//matching against a series of snapshots instead of a single shapshot
	//each type provides further qualifying extension prefix
	virtual FString GetSpecificExtensionPrefix() const override { return cExtensionPrefix; }
	
protected:
	//---- operation ----
	//specialised for dtw, not got shared API of classification or regression algorithms
	virtual void ResetModelInstance() override;
	virtual modelSetFloat* GetModelInstance() const override { /*not compatible with general model base*/ check(false); return nullptr; }
	//async
	virtual FInteractMLTask::Ptr BeginTrainingModel( class UInteractMLTrainingSet* training_set ) override;
	virtual void DoTrainingModel( FInteractMLTask::Ptr training_task ) override; //NOTE: Multi-threaded call, must be handled thread safely, only for direct training/running using task state
	virtual FInteractMLTask::Ptr BeginRunningModel(struct FInteractMLParameterSeries* parameter_series) override;
	virtual void DoRunningModel( FInteractMLTask::Ptr run_task ) override; //NOTE: Multi-threaded call, must be handled thread safely, only for direct training/running using task state

private:
	
	bool ApplyExamples();

};
