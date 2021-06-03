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
		
	///////////////////// PARAMETER COLLECTION //////////////////////
		
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


	///////////////////// TRAINING SET //////////////////////
	
	// training set access
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static UInteractMLTrainingSet* GetTrainingSet(AActor* Actor, FString DataPath, FString NodeID);
	// training set recording
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static bool RecordExample(UInteractMLTrainingSet* TrainingSet, FInteractMLParameters Parameters, int Label, bool Record, int Mode, FString NodeID);


	///////////////////// DEBUG/DIAGS //////////////////////

	// log a parameter collections list of parameter values
	UFUNCTION(BlueprintCallable,meta=(DevelopmentOnly))
	static void LogParameterCollection(FInteractMLParameters Parameters);
};
