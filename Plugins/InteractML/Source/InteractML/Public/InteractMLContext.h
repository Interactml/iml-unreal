//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

//module
#include "InteractMLTrainingSet.h"
#include "InteractMLModel.h"
#include "InteractMLParameters.h"
#include "InteractMLModelState.h"
#include "InteractMLContext.generated.h"


//general declarations


// InteractML operating context for ML graphs within actors
// Injected into containing actor to provide session state for running the ML nodes in the graph
// Non-persistent, transient objects
//
UCLASS()
class INTERACTML_API UInteractMLContext
	: public UActorComponent
{
	GENERATED_BODY()

public:		
	// whatever is used to uniquely identify each graph node
	typedef FString TGraphNodeID;

private:
	
	//cache of various objects (uproperties so they are visible to the engine)
	UPROPERTY()
	TArray<UInteractMLTrainingSet*>    TrainingSetCache;
	UPROPERTY()
	TArray<UInteractMLModel*>          ModelCache;

	//cache of raw object (non UObject)
	TArray<TSharedPtr<FInteractMLParameterCollection>>             ParametersCache;
	
	//lookup accellerators
	TMap<TGraphNodeID, UInteractMLTrainingSet*>                    TrainingSetLookup;
	TMap<TGraphNodeID, UInteractMLModel*>                          ModelLookup;
	TMap<TGraphNodeID, TSharedPtr<FInteractMLParameterCollection>> ParametersLookup;
	TMap<TGraphNodeID, TSharedPtr<FInteractMLModelState>>          StateLookup;
	
public:
	
	//---- access ----

	// fetch/re-fetch a training set for a specific graph node
	UInteractMLTrainingSet* GetTrainingSet( TGraphNodeID node_id, FString data_path );
	
	// fetch/re-fetch a model for a specific graph node
	UInteractMLModel* GetModel( UClass* model_type, TGraphNodeID node_id, FString data_path );

	// fetch/re-fetch a parameter set for passing around the graph
	TSharedPtr<FInteractMLParameterCollection> GetParameters( TGraphNodeID node_id );
	
	// fetch/re-fetch a model state object for a specific node to run a model
	TSharedPtr<FInteractMLModelState> GetModelState( TGraphNodeID node_id );

	//---- notification ----

	// notify use of a training set asset
	void SetTrainingSet( TGraphNodeID node_id, UInteractMLTrainingSet* training_set );
	
	// notify use of a model asset
	void SetModel( TGraphNodeID node_id, UInteractMLModel* model );
	
private:

};
