//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLStorage.h"
#include "InteractMLModel.generated.h"

//general declarations

// InteractML Model Base
// represents a trained machine learning model for specific algorithm types to derive from
// holds in-memory version of model instance, trained model state backed by underlying JSON file storage
//
UCLASS()
class INTERACTML_API UInteractMLModel
	: public UInteractMLStorage
{
	GENERATED_BODY()	
		
	//---- persistent state ----
	
	
	//---- transient/cached state ----
	
	
public:
		//---- constants ----
	
	// extension prefix for model data files
	static FString cExtensionPrefix;
	
	//---- access ----
	
	//each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const { return cExtensionPrefix; }
	
	
private:
	
};
