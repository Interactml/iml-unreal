//----
// InteractML - University Of Arts London
//----

#pragma once


//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLStorage.generated.h"

//general declarations


// InteractML data storage base class
// Handles interaction with the underlying JSON data file used to persist the ML data (training or trained model))
//
UCLASS()
class INTERACTML_API UInteractMLStorage
	: public UObject
{
	GENERATED_BODY()
		
	//---- persistent state ----

	//hard link to underlying data file
	UPROPERTY()
	uint64 FileID;

	
	//---- transient/cached state ----

	//actual file path
	FString FilePath;


public:
	//access



private:

};
