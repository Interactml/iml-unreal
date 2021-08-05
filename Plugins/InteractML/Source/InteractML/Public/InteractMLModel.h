//----
// InteractML - University Of Arts London
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

protected:
	bool bIsTrained;

public:	
	//tracking of incoming triggers
	FNodeActionInterlock TrainingRequest;	//node currently training this model
	FNodeActionInterlock ResetRequest;		//node currently resetting this model

	//tracking of internal status
	TSharedPtr<struct FInteractMLTask> TrainingTask; //currently active training task (NOTE: training task is tracked in model as can only be one training operation active at a time, running is tracked externally as there can be multiple active at once)
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
	bool RunModel( struct FInteractMLParameterCollection* parameters, TArray<float>& out_values );		//single sample
	bool RunModel( struct FInteractMLParameterSeries* parameter_series, TArray<float>& out_values );	//series of samples
	void TrainModel(class UInteractMLTrainingSet* training_set);
	void TrainModelAsync(class UInteractMLTrainingSet* training_set);
	void ResetModel();
	void HandleCompletedTask( TSharedPtr<FInteractMLTask> task );
	
	//---- persistence ----
	virtual void Create() override; //nothing to load, created a new one
	virtual bool LoadJson(const FString& json_string) override;
	virtual bool SaveJson(FString& json_string) const override;
	
	//type of model
	virtual bool IsSeries() const { return false; }	//matching against a series of snapshots instead of a single shapshot?
	virtual bool IsContinuous() const { return false; }	//produces interpolated outputs instead of discreet values?
	// each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const override { return GetSpecificExtensionPrefix() + cExtensionPrefix; }
	//each type provides further qualifying extension prefix
	virtual FString GetSpecificExtensionPrefix() const { check(false); return ""; }

	//derived
	bool IsSingle() const { return !IsSeries(); }	//matching against a single snapshot of samples instead of a series?
	bool IsDiscrete() const { return !IsContinuous(); } //produces discrete value outputs instead of interpolated?
	
protected:
	//---- per model type specialisations ----
	virtual bool RunModelInstance(struct FInteractMLParameterCollection* parameters, TArray<float>& out_values);
	virtual bool RunModelInstance(struct FInteractMLParameterSeries* parameter_series, TArray<float>& out_values) { check(false); return 0; }
	virtual void ResetModelInstance() { check(false); } //must override
	virtual modelSetFloat* GetModelInstance() const { check(false); return nullptr; } //must override
	//async
	friend struct FInteractMLTask;
	virtual TSharedPtr<FInteractMLTask> BeginTrainingModel( class UInteractMLTrainingSet* training_set );
	virtual void DoTrainingModel( TSharedPtr<FInteractMLTask> training_task ); //NOTE: Multi-threaded call, must be handled thread safely, only for direct training/running using task state
	virtual void EndTrainingModel( TSharedPtr<FInteractMLTask> training_task );
	
private:
	
};
