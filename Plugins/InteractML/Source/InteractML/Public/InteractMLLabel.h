//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"

//module
#include "InteractMLHelpers.h"
#include "InteractMLLabel.generated.h"

//general declarations


// InteractML Label type (structure definition)
// List of name/type pairs defining the set of values to be associated together as a label
// Used for specifying expected output when recording/training and produced as actual output on a running model
// NOTE: Just a simple specialisation of Blueprint Struct asset type, we want all the functionality/features but don't really need to add much
//
UCLASS()
class INTERACTML_API UInteractMLLabel : public UUserDefinedStruct
{
	GENERATED_BODY()	
};
