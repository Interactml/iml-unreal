//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLHelpers.h"
#include "InteractMLParameters.h"
//#include "InteractMLModelState.generated.h"


//general declarations


// InteractML per-node state for running models (e.g. separate accumulation of series)
// sometimes needed so that multiple nodes can run the same model independantly
// obtained/cached by UInteractMLContext object in the nodes acto
//
class INTERACTML_API FInteractMLModelState
{
public:		
	// node currently running this model
	FNodeActionInterlock RunAction;
	
	// parameter sample accumulation
	FInteractMLParameterSeries ParameterSeries;

	// last result calculated by running the model
	TArray<float> CurrentResult;


	// reset to initial state
	void Reset();
};
