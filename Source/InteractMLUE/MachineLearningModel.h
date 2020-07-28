// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MachineLearningModel.generated.h"

USTRUCT(BlueprintType)
struct INTERACTMLUE_API FDataInstance
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
	TArray<float> inputs;
	UPROPERTY()
	TArray<float> outputs;

};


UCLASS(Blueprintable, ClassGroup=(InteractML), meta=(BlueprintSpawnableComponent) )
class INTERACTMLUE_API UMachineLearningModel : public UActorComponent
{
	GENERATED_BODY()

	typedef void* (*_createModel)(); // Declare a method to store the DLL method createRegressionModel.
	_createModel m_createRegressionModelFromDLL;
	
	typedef void* (*_createModel)(); // Declare a method to store the DLL method createRegressionModel.
	_createModel m_createClassificationModelFromDLL;

	typedef const char* (*_getJSON)(void*); // Declare a method to store the DLL method createRegressionModel.
	_getJSON m_getJSONFromDLL;

	typedef void (*_destroyModel)(void*); // Declare a method to store the DLL method createRegressionModel.
	_destroyModel m_destroyModelFromDLL;

	typedef void* (*_createTrainingSet)();
	_createTrainingSet m_createTrainingSetFromDLL;

	typedef void (*_destroyTrainingSet)(void*);
	_destroyTrainingSet m_destroyTrainingSetFromDLL;

	typedef void (*_addTrainingExample)(void*, double *inputs, int numInputs, double * outputs, int numOutputs);
	_addTrainingExample m_addTrainingExampleFromDLL;

	typedef bool (*_trainRegression) (void*, void*);
	_trainRegression m_trainRegressionFromDLL;

	typedef bool (*_trainClassification) (void*, void*);
	_trainClassification m_trainClassificationFromDLL;

	typedef int (*_run)(void*, double* inputs, int numInputs, double* outputs, int numOutputs);
	_run m_runFromDLL;
	

	void* v_dllHandle;

	int m_numInputs = -1;
	int m_numOutputs = -1;

public:	


	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine Learning")
	void *m_modelPtr = NULL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine Learning")
	TArray<FDataInstance> m_dataset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
	FString m_dllFolder = "RapidLib";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
		FString m_dllName = "RapidLibPlugin.dll";




	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool importDLL(FString folder, FString name);

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	void freeDLL();

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool IsDLLLoaded();

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool addTrainingData(TArray<float> input, TArray<float> output);
	

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool trainRegressor();
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool trainClassifier();

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool IsModelInitialised();

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	TArray<float> Run(TArray<float> input);


	// Sets default values for this component's properties
	UMachineLearningModel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// Called when the game starts
	virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
