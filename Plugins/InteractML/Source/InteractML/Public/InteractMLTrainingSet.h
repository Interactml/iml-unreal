//----
// InteractML - University Of Arts London
//----

#pragma once


//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLStorage.h"
#include "InteractMLTrainingSet.generated.h"


//general declarations


// InteractML Training Set
// represents collection of training data used to train a model
// holds in-memory version of training set, backed by underlying JSON file storage
//
UCLASS()
class INTERACTML_API UInteractMLTrainingSet
	: public UInteractMLStorage
{
	GENERATED_BODY()
		
	//---- persistent state ----
	
	
	//---- transient/cached state ----
	bool bIsRecording;
	int RecordingMode;
	
public:
	//---- constants ----
	
	// extension prefix for example/training data files
	static FString cExtensionPrefix;
	

	//---- access ----
	bool IsRecording() const { return bIsRecording; }
	
	//each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const { return cExtensionPrefix; }



private:

};
