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
	
	// parameter collection access
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static FInteractMLParameters GetParameters( AActor* Actor, FString NodeID );	
	// parameter accumulation
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddIntegerParameter( FInteractMLParameters Parameters, int Value );
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddFloatParameter( FInteractMLParameters Parameters, float Value );
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddBooleanParameter( FInteractMLParameters Parameters, bool Value );
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddVector2Parameter( FInteractMLParameters Parameters, FVector2D Value );
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddVector3Parameter( FInteractMLParameters Parameters, FVector Value );
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddQuaternionParameter( FInteractMLParameters Parameters, FQuat Value );
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void AddColourParameter( FInteractMLParameters Parameters, FLinearColor Value );
	

	UFUNCTION(BlueprintCallable)
	static void DebugParameterCollection(FInteractMLParameters Parameters);
};
