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
	static bool RecordExample(UInteractMLTrainingSet* TrainingSet, FInteractMLParameters Parameters, float Label, bool Record, bool Reset, int Mode, FString NodeID);
	

	///////////////////// MODEL //////////////////////
	
	// model access
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static UInteractMLModel* GetModel_Classification(AActor* Actor, FString DataPath, FString NodeID);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static UInteractMLModel* GetModel_Regression(AActor* Actor, FString DataPath, FString NodeID);
	//UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	//static UInteractMLModel* GetModel_DynamicTimeWarp(AActor* Actor, FString DataPath, FString NodeID);
	// model running
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static float RunModel(UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID);
	// model training
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static bool TrainModel(UInteractMLModel* Model, UInteractMLTrainingSet* TrainingSet, bool Train, bool Reset, FString NodeID);
	

	///////////////////// UTILITY //////////////////////

	//persistence
	UFUNCTION(BlueprintCallable, meta=(ReturnDisplayName = "Success", ToolTip="Write any unsaved example or model data to disk. Only needed in standalone application, or if you want to save early in an editor play session."))
	static bool Save();

	///////////////////// DEBUG/DIAGS //////////////////////

	// log a parameter collections list of parameter values
	UFUNCTION(BlueprintCallable,meta=(DevelopmentOnly))
	static void LogParameterCollection(FInteractMLParameters Parameters);
};
