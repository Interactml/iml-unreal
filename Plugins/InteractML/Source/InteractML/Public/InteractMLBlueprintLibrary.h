//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once


//unreal
#include "CoreMinimal.h"
//#include "SharedPointer.h"
#include "Kismet/KismetSystemLibrary.h"

//module
#include "InteractMLParameters.h"
#include "InteractMLBlueprintLibrary.generated.h"

//general declarations

// model types as an enumeration
//
UENUM(BlueprintType)
enum class EInteractMLModelType : uint8
{
	Classification,
	Regression,
	DynamicTimewarp,
};


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
	static UInteractMLTrainingSet* GetTrainingSet(AActor* Actor, FString DataPath, FString NodeID, bool& HasData);
	// training set recording : Simple label
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static bool RecordExampleSimple(AActor* Actor, UInteractMLTrainingSet* TrainingSet, FInteractMLParameters Parameters, bool WantSeries, float Label, bool Record, bool DeleteLast, bool DeleteLabel, bool DeleteAll, FString NodeID);
	// training set recording : Composite label
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(CustomStructureParam="LabelData"), BlueprintInternalUseOnly)
	static bool RecordExampleComposite(AActor* Actor, UInteractMLTrainingSet* TrainingSet, FInteractMLParameters Parameters, bool WantSeries, const UInteractMLLabel* LabelType, const FGenericStruct& LabelData, bool Record, bool DeleteLast, bool DeleteLabel, bool DeleteAll, FString NodeID);
	//generic handler for any UInteractMLLabel struct type
	static bool Generic_RecordExampleComposite(AActor* Actor, UInteractMLTrainingSet* TrainingSet, FInteractMLParameters Parameters, bool WantSeries, const UInteractMLLabel* LabelType, const void* LabelData, bool Record, bool DeleteLast, bool DeleteLabel, bool DeleteAll, FString NodeID);
	/** Based on UInteractMLBlueprintLibrary::execRecordExampleComposite */
	// AActor* Actor, 
	// UInteractMLTrainingSet* TrainingSet, 
	// FInteractMLParameters Parameters, 
	// bool WantSeries, 
	// const UInteractMLLabel* LabelType, 
	// const FGenericStruct& LabelData, 
	// bool Record, 
	// bool DeleteLast,
	// bool DeleteLabel,
	// bool DeleteAll,
	// FString NodeID);
	DECLARE_FUNCTION(execRecordExampleComposite)
	{
		P_GET_OBJECT(AActor, Actor);
		P_GET_OBJECT(UInteractMLTrainingSet, TrainingSet);
		Stack.StepCompiledIn<FStructProperty>(NULL); //P_GET_PROPERTY(FInteractMLParameters, Parameters); - dont' need named prop entry, structs passed via pointer
		FInteractMLParameters* ParametersStructAddr = (FInteractMLParameters*)Stack.MostRecentPropertyAddress;
		P_GET_PROPERTY(FBoolProperty, WantSeries);	
		P_GET_OBJECT(UInteractMLLabel, LabelType);
		Stack.StepCompiledIn<FStructProperty>(NULL); //P_GET_PROPERTY(FStructProperty, LabelData); - dont' need named prop entry, structs passed via pointer
		void* LabelDataStructAddr = Stack.MostRecentPropertyAddress;
		P_GET_PROPERTY(FBoolProperty, Record);	
		P_GET_PROPERTY(FBoolProperty, DeleteLast);	
		P_GET_PROPERTY(FBoolProperty, DeleteLabel);	
		P_GET_PROPERTY(FBoolProperty, DeleteAll);	
		P_GET_PROPERTY(FStrProperty, NodeID);	
		P_FINISH;

		bool bSuccess = false;
		P_NATIVE_BEGIN;
		bSuccess = Generic_RecordExampleComposite(Actor, TrainingSet, *ParametersStructAddr, WantSeries, LabelType, LabelDataStructAddr, Record, DeleteLast, DeleteLabel, DeleteAll, NodeID );
		P_NATIVE_END;
		
		*(bool*)RESULT_PARAM = bSuccess;
	}
	
	
	///////////////////// MODEL //////////////////////
	
	// model access
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static UInteractMLModel* GetModel(AActor* Actor, FString DataPath, EInteractMLModelType ModelType, FString NodeID, bool& IsTrained);

	// model running : simple label
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static float RunModelSimple(AActor* Actor, UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static float RunModelSimpleAsync(AActor* Actor, UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID, bool& Running, bool& Completed);
	// model running : composite label
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(CustomStructureParam="LabelData"), BlueprintInternalUseOnly)
	static void RunModelComposite(AActor* Actor, UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID, const UInteractMLLabel* LabelType, FGenericStruct& LabelData);
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(CustomStructureParam="LabelData"), BlueprintInternalUseOnly)
	static void RunModelCompositeAsync(AActor* Actor, UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID, const UInteractMLLabel* LabelType, FGenericStruct& LabelData, bool& Running, bool& Completed);
	
	// generic implementation of RunModelComposite
	//
	static void Generic_RunModelComposite(AActor* Actor, UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID, const UInteractMLLabel* LabelType, void* LabelData);
	/** Based on UInteractMLBlueprintLibrary::execRunModelComposite */
	//AActor* Actor, 
	//UInteractMLModel* Model, 
	//FInteractMLParameters Parameters, 
	//bool Run, 
	//FString NodeID, 
	//const UInteractMLLabel* LabelType, 
	//FGenericStruct& LabelData);
	DECLARE_FUNCTION(execRunModelComposite)
	{
		P_GET_OBJECT(AActor, Actor);
		P_GET_OBJECT(UInteractMLModel, Model);
		Stack.StepCompiledIn<FStructProperty>(NULL); //P_GET_PROPERTY(FInteractMLParameters, Parameters); - dont' need named prop entry, structs passed via pointer
		FInteractMLParameters* ParametersStructAddr = (FInteractMLParameters*)Stack.MostRecentPropertyAddress;
		P_GET_PROPERTY(FBoolProperty, Run);
		P_GET_PROPERTY(FStrProperty, NodeID);
		P_GET_OBJECT(UInteractMLLabel, LabelType);
		Stack.StepCompiledIn<FStructProperty>(NULL); //P_GET_PROPERTY(FStructProperty, LabelData); - dont' need named prop entry, structs passed via pointer
		void* LabelDataStructAddr = Stack.MostRecentPropertyAddress;
		P_FINISH;
		
		P_NATIVE_BEGIN;
		Generic_RunModelComposite(Actor, Model, *ParametersStructAddr, Run, NodeID, LabelType, LabelDataStructAddr);
		P_NATIVE_END;
	}
	
	// generic implementation of RunModelCompositeAsync
	//
	static void Generic_RunModelCompositeAsync(AActor* Actor, UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID, const UInteractMLLabel* LabelType, void* LabelData, bool& Running, bool& Completed);
	/** Based on UInteractMLBlueprintLibrary::execRunModelComposite */
	//AActor* Actor, 
	//UInteractMLModel* Model, 
	//FInteractMLParameters Parameters, 
	//bool Run, 
	//FString NodeID, 
	//const UInteractMLLabel* LabelType, 
	//FGenericStruct& LabelData,
	//bool& Running,
	//bool& Completed);
	DECLARE_FUNCTION(execRunModelCompositeAsync)
	{
		P_GET_OBJECT(AActor, Actor);
		P_GET_OBJECT(UInteractMLModel, Model);
		Stack.StepCompiledIn<FStructProperty>(NULL); //P_GET_PROPERTY(FInteractMLParameters, Parameters); - dont' need named prop entry, structs passed via pointer
		FInteractMLParameters* ParametersStructAddr = (FInteractMLParameters*)Stack.MostRecentPropertyAddress;
		P_GET_PROPERTY(FBoolProperty, Run);
		P_GET_PROPERTY(FStrProperty, NodeID);
		P_GET_OBJECT(UInteractMLLabel, LabelType);
		Stack.StepCompiledIn<FStructProperty>(NULL); //P_GET_PROPERTY(FStructProperty, LabelData); - dont' need named prop entry, structs passed via pointer
		void* LabelDataStructAddr = Stack.MostRecentPropertyAddress;
		P_GET_UBOOL_REF(Out_Running);
		P_GET_UBOOL_REF(Out_Completed);
		P_FINISH;
		
		P_NATIVE_BEGIN;
		Generic_RunModelCompositeAsync(Actor, Model, *ParametersStructAddr, Run, NodeID, LabelType, LabelDataStructAddr, Out_Running, Out_Completed);
		P_NATIVE_END;
	}	

	// model training : blocking
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static bool TrainModel(AActor* Actor, UInteractMLModel* Model, UInteractMLTrainingSet* TrainingSet, bool Train, bool Reset, FString NodeID);
	// model training : asynchronous
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static bool TrainModelAsync(AActor* Actor, UInteractMLModel* Model, UInteractMLTrainingSet* TrainingSet, bool Train, bool Reset, FString NodeID, bool& Training, bool& Completed );

	
	///////////////////// UTILITY //////////////////////

	//persistence
	UFUNCTION(BlueprintCallable, Category="InteractML", meta=(ReturnDisplayName = "Success", ToolTip="Write any unsaved example or model data to disk. Only needed in standalone application, or if you want to save early in an editor play session."))
	static bool Save();

	//persistence
	UFUNCTION( BlueprintCallable, Category = "Utility", meta = (ReturnDisplayName = "Value", ToolTip = "Access an objects float property by name.") )
	static float GetFloatProperty( UObject* Target, FName Name );


	///////////////////// DEBUG/DIAGS //////////////////////

	// log a parameter collections list of parameter values
	UFUNCTION(BlueprintCallable, Category = "InteractML",meta=(DevelopmentOnly))
	static void LogParameterCollection(FInteractMLParameters Parameters);

};
