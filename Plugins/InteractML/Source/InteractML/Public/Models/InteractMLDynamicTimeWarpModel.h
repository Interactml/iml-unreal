//----
// InteractML - University Of Arts London
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
UCLASS()
class INTERACTML_API UInteractMLDynamicTimeWarpModel
	: public UInteractMLModel
{
	GENERATED_BODY()	
		
	//---- persistent state ----
	//the trained state (for DTW it is the same as the training data so we just persist a copy)
	UPROPERTY()
	TArray<FInteractMLExample> Examples;
	
	
	//---- transient/cached state ----
	TSharedPtr<seriesClassificationFloat> Model;
	

public:
	//---- constants ----
	
	// extension prefix for model data files
	static FString cExtensionPrefix;
	
	//---- access ----

	//---- persistence ----
	virtual bool LoadJson(const FString& json_string) override;
	virtual bool SaveJson(FString& json_string) const override;
	
	//---- operation ----
	//specialised for dtw, not got shared API of classification or regression algorithms
	virtual float RunModelInstance(struct FInteractMLParameterSeries* parameters) override;
	virtual bool TrainModelInstance(class UInteractMLTrainingSet* training_set) override;
	virtual void ResetModelInstance() override;
	virtual modelSetFloat* GetModelInstance() const override { /*not compatible with general model base*/ check(false); return nullptr; }
	
	//each type provides further qualifying extension prefix
	virtual bool IsSeries() const { return true; }	//matching against a series of snapshots instead of a single shapshot?
	virtual FString GetSpecificExtensionPrefix() const override { return cExtensionPrefix; }
	

private:
	
	bool ApplyExamples();

};
