//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLTrainingSet.h"
#include "InteractMLModel.h"
#include "InteractMLParameters.h"
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
	TArray<TSharedPtr<FInteractMLParameters>> ParametersCache;
	
	//lookup accellerators
	TMap<TGraphNodeID, UInteractMLTrainingSet*>           TrainingSetLookup;
	TMap<TGraphNodeID, UInteractMLModel*>                 ModelLookup;
	TMap<TGraphNodeID, TSharedPtr<FInteractMLParameters>> ParametersLookup;
	
public:
	
	//---- access ----

	// fetch/re-fetch a training set for a specific graph node
	UInteractMLTrainingSet* GetTrainingSet( TGraphNodeID node_id, FString data_path );
	
	// fetch/re-fetch a model for a specific graph node
	UInteractMLModel* GetModel( TGraphNodeID node_id, FString data_path );

	// fetch/re-fetch a parameter set for passing around the graph
	TSharedPtr<FInteractMLParameters> GetParameters( TGraphNodeID node_id );
	

private:

};
