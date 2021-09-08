//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//rapidlib
#include "modelSet.h"
using modelSetFloat = modelSet<float>;

//module
#include "InteractMLStorage.h"
#include "InteractMLHelpers.h"
#include "InteractMLLabelCache.h"
#include "InteractMLTask.h"
#include "InteractMLModel.generated.h"

//general declarations

// InteractML Model Base
// represents a trained machine learning model for specific algorithm types to derive from
// holds in-memory version of model instance, trained model state backed by underlying JSON file storage
// NOTE: apart from the actual training/running (DoTrainingModel/DoRunningModel) all async operations and related state changes are handled on main thread so no thread safety concerns are warranted
//
UCLASS(Abstract,BlueprintType)
class INTERACTML_API UInteractMLModel
	: public UInteractMLStorage
{
	GENERATED_BODY()	
		
	//---- persistent state ----
	
	//label information
	UPROPERTY()
	FInteractMLLabelCache LabelCache;

	//---- transient/cached state ----

	//task currently running this model (used to spot concurrency requests)
	TArray<FInteractMLTask::Ptr> RunTasks;

protected:
	bool bIsTrained;

public:	
	//tracking of incoming triggers
	FNodeActionInterlock TrainingRequest;	//node currently training this model
	FNodeActionInterlock ResetRequest;		//node currently resetting this model

	//tracking of internal status
	FInteractMLTask::Ptr TrainingTask; //currently active training task (NOTE: training task is tracked in model as can only be one training operation active at a time, running is tracked externally as there can be multiple active at once)
	mutable bool bTrainingCompleted;				 //node just finished training asynchronously (only stays true until checked)
	
	//---- constants ----
	
	// extension prefix part used for ALL model data files
	static FString cExtensionPrefix;
	
	//---- access ----
	bool IsTrained() const { return bIsTrained; } //has been trained in the past, model is runnable, not true during async training
	bool IsTraining() const { return TrainingTask.IsValid(); } //background training in progress
	bool CheckJustCompleted() const { bool completed = bTrainingCompleted; bTrainingCompleted = false; return completed; } //background training has just completed, ready to use
	const FInteractMLLabelCache& GetLabelCache() const { return LabelCache; }
	
	//---- operation ----
	bool RunModel( struct FInteractMLParameterCollection* parameters, TArray<float>& output );			//single sample
	bool RunModel( struct FInteractMLParameterSeries* parameter_series, TArray<float>& output );		//series of samples
	FInteractMLTask::Ptr RunModelAsync( struct FInteractMLParameterCollection* parameters );		//single sample
	FInteractMLTask::Ptr RunModelAsync( struct FInteractMLParameterSeries* parameter_series );	//series of samples
	void TrainModel(class UInteractMLTrainingSet* training_set);
	void TrainModelAsync(class UInteractMLTrainingSet* training_set);
	void ResetModel();
	void HandleCompletedTask( FInteractMLTask::Ptr task );
	
	//---- persistence ----
	virtual void Create() override; //nothing to load, created a new one
	virtual bool LoadJson(const FString& json_string) override;
	virtual bool SaveJson(FString& json_string) const override;

	//---- access ----
	
	//type of model
	virtual bool IsSeries() const { return false; }	//matching against a series of snapshots instead of a single shapshot?
	virtual bool IsContinuous() const { return false; }	//produces interpolated outputs instead of discreet values?
	virtual bool CanRunConcurrently() const { return false; } //can we run this model concurrently? (default to no, algorithms known to be thread-safe can return true)
	// each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const override { return GetSpecificExtensionPrefix() + cExtensionPrefix; }
	//each type provides further qualifying extension prefix
	virtual FString GetSpecificExtensionPrefix() const { check(false); return ""; }
	
	//derived
	bool IsSingle() const { return !IsSeries(); }	//matching against a single snapshot of samples instead of a series?
	bool IsDiscrete() const { return !IsContinuous(); } //produces discrete value outputs instead of interpolated?
	
protected:
	//---- per model type specialisations ----
	virtual void ResetModelInstance() { check(false); } //must override
	virtual modelSetFloat* GetModelInstance() const { check(false); return nullptr; } //must override (where applicable to default implementation provided here)
	//async
	friend struct FInteractMLTask;
	virtual FInteractMLTask::Ptr BeginTrainingModel( class UInteractMLTrainingSet* training_set );
	virtual void DoTrainingModel( FInteractMLTask::Ptr train_task ); //NOTE: Multi-threaded call, must be handled thread safely, only for direct training/running using task state
	virtual void EndTrainingModel( FInteractMLTask::Ptr train_task );
	virtual FInteractMLTask::Ptr BeginRunningModel( struct FInteractMLParameterCollection* parameters );
	virtual FInteractMLTask::Ptr BeginRunningModel( struct FInteractMLParameterSeries* parameter_series ) { check(false); return 0; }
	virtual void DoRunningModel( FInteractMLTask::Ptr run_task ); //NOTE: Multi-threaded call, must be handled thread safely, only for direct training/running using task state
	virtual void EndRunningModel( FInteractMLTask::Ptr run_task );	
	//tracking of tasks running this model
	bool CheckAddTask(FInteractMLTask::Ptr task);	//returns true if you are allowed to run this task
	void CheckRemoveTask(FInteractMLTask::Ptr task);

private:
	
};
