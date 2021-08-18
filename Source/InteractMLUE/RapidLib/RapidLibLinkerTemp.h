// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Components/ActorComponent.h"
#include "../DataSerialization.h"



class INTERACTMLUE_API FRapidLibLinkerDLLTemp : public UActorComponent
{
	//REGRESSION
	typedef void* (*_createRegressionModel)(); // Declare a method to store the DLL method createRegressionModel.
	_createRegressionModel m_createRegressionModelFromDLL;

	typedef bool (*_trainRegression) (void*, void*);
	_trainRegression m_trainRegressionFromDLL;

	//CLASSIFICATION

	typedef void* (*_createClassificationModel)(); // Declare a method to store the DLL method createRegressionModel.
	_createClassificationModel m_createClassificationModelFromDLL;

	typedef bool (*_trainClassification) (void*, void*);
	 _trainClassification m_trainClassificationFromDLL;

	//DTW

	typedef void* (*_createSeriesClassificationModel)(); // Declare a method to store the DLL method createRegressionModel.
	_createSeriesClassificationModel m_createSeriesClassificationModelFromDLL;

	typedef bool (*_trainSeriesClassification) (void*, void*);// train DTW
	_trainSeriesClassification m_trainDTWFromDLL;

	typedef bool (*_resetSeriesClassification) (void*); // reset dtw model
	_resetSeriesClassification m_resetDTW;

	typedef void (*_destroySeriesClassificationModel)(void*); // destroy dtw model
	_destroySeriesClassificationModel m_destroyDTW;

	typedef void* (*_runSeriesClassification)(void*, void*); // run dtw model
	_runSeriesClassification _runDTW;

	typedef int* (*_getSeriesClassificationCosts)(void*, double* output, int numOutputs); // calculate costs of dtw model
	_getSeriesClassificationCosts _dtwCost;


	// GENERIC

	typedef int* (*_process)(void*, double* input, int numInputs, double* output, int numOutputs);
	_process _processModel;

	typedef const char* (*_getJSON)(void*); // Declare a method to store the DLL method createRegressionModel.
	_getJSON m_getJSONFromDLL;

	typedef const void (*_putJSON)(void*, const char jsonString); // Declare a method to store the DLL method createRegressionModel.
	_putJSON m_putJSONDLL;

	typedef void (*_destroyModel)(void*); // Declare a method to store the DLL method createRegressionModel.
	_destroyModel m_destroyModelFromDLL;

	// TRAINING  EXAMPLES

	typedef void* (*_createTrainingSet)();
	_createTrainingSet m_createTrainingSetFromDLL;

	typedef void (*_destroyTrainingSet)(void*);
	_destroyTrainingSet m_destroyTrainingSetFromDLL;

	typedef void (*_addTrainingExample)(void*, double* inputs, int numInputs, double* outputs, int numOutputs);
	_addTrainingExample m_addTrainingExampleFromDLL;

	typedef int* (*_getNumTrainingExamples)(void*);
	_getNumTrainingExamples m_getNumTrainingExamples;

	typedef double* (*_getInput)(void*, int i, int j);
	_getInput m_getInput;

	typedef double* (*_getOutput)(void*, int i, int j);
	_getOutput m_getOutput;

	// TRAINING SERIES 

	typedef void* (*_createTrainingSeries)();
	_createTrainingSeries m_createTrainingSeriesFromDLL;

	typedef void (*_destroyTrainingSeries)(void*);
	_destroyTrainingSeries m_destroyTrainingSeries;

	typedef void* (*_addInputsToSeries)(void*, double* inputs, int numInputs);
	_addInputsToSeries m_addInputsToSeries;

	typedef void* (*_addLabelToSeries)(void*, const char labelString);
	_addLabelToSeries m_addLabelToSeries;

	typedef void* (*_createTrainingSeriesCollection)();
	_createTrainingSeriesCollection m_createTrainingSeriesCollection;

	typedef void(*_destroyTrainingSeriesCollection)(void*);
	_destroyTrainingSeriesCollection m_destroyTrainingSeriesCollection;

	typedef void(*_addSeriesToSeriesCollection)(void*, void*);
	_addSeriesToSeriesCollection m_addSeriesToSeriesCollection;

	//RUN 

	typedef int (*_run)(void*, double* inputs, int numInputs, double* outputs, int numOutputs);
	_run m_runFromDLL;
	

	void* v_dllHandle;


public:	


	FString m_dllFolder = "RapidLib";

	FString m_dllName = "RapidLibPlugin.dll";


	bool importDLL(FString folder, FString name);

	void freeDLL();

	bool IsDLLLoaded();


	// Sets default values for this component's properties
	FRapidLibLinkerDLLTemp();

		
};
