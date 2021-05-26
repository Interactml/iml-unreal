// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//rapidlib
#include "trainingExample.h"
#include "regression.h"
#include "classification.h"
#include "seriesClassification.h"

//unreal
#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Components/ActorComponent.h"
#include "DataSerialization.h"
#include "MachineLearningModel.generated.h"

USTRUCT(BlueprintType)
struct INTERACTMLUE_API FDataInstance
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<float> inputs;
	UPROPERTY()
	TArray<float> outputs;

};

USTRUCT(BlueprintType)
struct INTERACTMLUE_API FDataInstanceSeriesMember
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
	TArray<float> inputData;

	FDataInstanceSeriesMember() {

	}


	FDataInstanceSeriesMember(TArray<float> inputArray) {
		inputData = inputArray;
	}

};

USTRUCT(BlueprintType)
struct INTERACTMLUE_API FDataInstanceSeries
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
	TArray<FDataInstanceSeriesMember> inputSeries;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
	FString label;
};



UCLASS(Blueprintable, ClassGroup=(InteractML), meta=(BlueprintSpawnableComponent) )
class INTERACTMLUE_API UMachineLearningModel : public UActorComponent
{
	GENERATED_BODY()
#if 0
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

	typedef void (*_addTrainingExample)(void*, double *inputs, int numInputs, double *outputs, int numOutputs);
	_addTrainingExample m_addTrainingExampleFromDLL;

	typedef bool (*_trainRegression) (void*, void*);
	_trainRegression m_trainRegressionFromDLL;

	typedef bool (*_trainClassification) (void*, void*);
	_trainClassification m_trainClassificationFromDLL;

	typedef int (*_run)(void*, double *inputs, int numInputs, double *outputs, int numOutputs);
	_run m_runFromDLL;
	
	typedef void* (*_createSeriesClassificationModel)(); // Declare a method to store the DLL method createRegressionModel.
	_createSeriesClassificationModel m_createDTWModelFromDLL;

	typedef bool (*_trainSeriesClassification) (void*, void*);// train DTW
	_trainSeriesClassification m_trainDTWFromDLL;

	typedef bool (*_resetSeriesClassification) (void*); // reset dtw model
	_resetSeriesClassification m_resetDTW;

	typedef void (*_destroySeriesClassificationModel)(void*); // destroy dtw model
	_destroySeriesClassificationModel m_destroyDTW;

	typedef void* (*_runSeriesClassification)(void*, void*); // run dtw model
	_runSeriesClassification m_runDTW;

	typedef int* (*_getSeriesClassificationCosts)(void*, double *output, int numOutputs); // calculate costs of dtw model
	_getSeriesClassificationCosts m_dtwCost;
	
	typedef int* (*_createTrainingSeriesCollection)(); // create training series collection
	_createTrainingSeriesCollection m_dtwTrainingSeriesCollection;
	
	typedef int* (*_createTrainingSeries)(); // create training series example
	_createTrainingSeries m_dtwTrainingSeries;
	
	//add example to the dtw series collection param 1 address to add to param 2 address to add
	typedef void* (*_addSeriesToSeriesCollection)(void*, void*); 
	_addSeriesToSeriesCollection m_addSeriesToDTWCollection;
	
	// destroy series training set 
	typedef void* (*_destroyTrainingSeriesCollection)(void*);
	_destroyTrainingSeriesCollection m_destroyDTWDLLTrainingSet;
	
	typedef void* (*_addInputsToSeries)(void*, double *inputs, int numInputs); // create training series 
	_addInputsToSeries m_addToSeries;
	
	typedef void* (*_addLabelToSeries)(void*, const TCHAR* label); // create training series 
	_addLabelToSeries m_addLabelDTW;
	

	void* v_dllHandle;
#endif
	int m_numInputs = -1;
	int m_numOutputs = -1;
	
	bool collecting = false; 

public:	

	//models
	regression *m_regressionModel = nullptr;
	classification *m_classificationModel = nullptr;
	seriesClassification *m_dtwModel = nullptr;

	// bool is the ML currently running
	bool running;

	//potentially delete
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine Learning")
	void *m_modelPtr = NULL;

	/// <summary>
	/// Array of training examples for regression/ classification
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
	TArray<FDataInstance> m_dataset;

	/// <summary>
	/// Array of training example series for DTW
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
	TArray<FDataInstanceSeries> m_datasetSeries;

	/// <summary>
	/// Single series for holding current DTW training recording 
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "Machine Learning")
	FDataInstanceSeries m_datasetSeriesSingle;
	
	/// <summary>
	/// Single series for hold the current frame of dtw
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "Machine Learning")
	FDataInstanceSeriesMember m_datasetSeriesSingleMember;

	std::vector<std::vector<double>> m_populatingSet;


	UPROPERTY(VisibleAnywhere, Category = "Machine Learning")
	FString m_TrainingExamplesFolder = "Data";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Machine Learning")
	FString filepath = *FPaths::ProjectContentDir() + m_TrainingExamplesFolder + "/";

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool addTrainingData(TArray<float> input, TArray<float> output);
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool addTrainingDataSeries(FDataInstanceSeriesMember trainingSeriesMember, FString label);
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool stopCollecting();

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool trainRegressor();
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool trainClassifier();
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool trainDTW(TArray<FDataInstanceSeries> trainingDataSeries);

	bool IsModelInitialised();

	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	TArray<float> Run(TArray<float> input);
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool SetUpInputsOutputs(FDataInstance example);
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool SetUpInputsOutputsSerie(FDataInstanceSeriesMember example, FString label);
	
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	FDataInstanceSeriesMember SetUpSeriesStruct(TArray<float> inputs);
	
	/// <summary>
	/// Stops populating the series for dtw
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	FString StopPopulatingDTW();
	
	/// <summary>
	/// Logic for whilst populating is happening 
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Machine Learning")
	bool PopulatingLogic(TArray<float> input);

	void BuildTrainingSetFromDataSet(TArray<FDataInstance>& dataset, std::vector<trainingExample>& training_set_out);

	// Sets default values for this component's properties
	UMachineLearningModel();


	//temp JSON handling
	UFUNCTION( BlueprintCallable, Category = "Machine Learning" )
	void SaveDatasetAsJson( FString file_path, bool& success );
	UFUNCTION( BlueprintCallable, Category = "Machine Learning" )
	void LoadDatasetFromJson( FString file_path, bool& success );

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// Called when the game starts
	virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	
};
