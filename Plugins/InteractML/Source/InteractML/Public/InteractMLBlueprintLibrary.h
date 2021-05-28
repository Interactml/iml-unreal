//----
// InteractML - University Of Arts London
//----

#pragma once


//unreal
#include "CoreMinimal.h"
#include "SharedPointer.h"

//module
#include "InteractMLParameters.h"
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
	static UInteractMLContext* GetMLContext(AActor* Actor);

	// parameter access
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static FInteractMLParametersPtr GetParameters( UInteractMLContext* Context, FString NodeID );	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddFloatParameter( FInteractMLParametersPtr Parameters, float Value );
	
};
