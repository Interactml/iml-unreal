//----
// InteractML - University Of Arts London
//----

#pragma once


//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLBlueprintLibrary.generated.h"

//general declarations


// InteractML blueprint support library
// * Internal - Various functions for building our custom blueprint nodes from
// * Public - We may also include helper functions for users to build into their graphs
//
UCLASS()
class INTERACTML_API UInteractMLBlueprintLibrary
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	// context access for blue graph running in any actor
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"))
	UInteractMLContext* GetMLContext(AActor* Actor);


	// test
	UFUNCTION(BlueprintCallable, Category="InteractML")
	void TestIML(AActor* Actor);
	
	
};
