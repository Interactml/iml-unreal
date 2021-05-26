//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "Kismet/BlueprintFunctionLibrary.h"

//module
#include "BlueprintLibrary.generated.h"


// InteractML blueprint function library
//
UCLASS()
class INTERACTML_API UInteractMLBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//IML Test function, does nothing
	UFUNCTION(BlueprintCallable, Category="InteractML")
	static int DoNothing( int i, float f );
};