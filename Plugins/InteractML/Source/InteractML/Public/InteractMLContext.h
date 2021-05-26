//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//module
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
	//access



private:

};
