//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//reactml
#include "regression.h"

//module
#include "InteractMLModel.h"
#include "InteractMLRegressionModel.generated.h"

//general declarations

// InteractML Model for running a Regression algorithm
//
UCLASS(BlueprintType)
class INTERACTML_API UInteractMLRegressionModel
	: public UInteractMLModel
{
	GENERATED_BODY()	
		
	//---- persistent state ----
	
	//---- transient/cached state ----
	TSharedPtr<regressionFloat, ESPMode::ThreadSafe> Model;
	
public:
	//---- constants ----
	
	// extension prefix for model data files
	static FString cExtensionPrefix;
	
	//---- access ----

	//---- operation ----
	virtual void ResetModelInstance() override;
	virtual modelSetFloat* GetModelInstance() const override { return Model.Get(); }

	//model type	
	virtual bool IsContinuous() const { return true; }	//produces interpolated outputs instead of discreet values
	//each type provides further qualifying extension prefix
	virtual FString GetSpecificExtensionPrefix() const override { return cExtensionPrefix; }
	

private:
	
};
