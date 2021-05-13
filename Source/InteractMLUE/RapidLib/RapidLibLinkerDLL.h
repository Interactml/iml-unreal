// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>

namespace InteractML {

	class INTERACTMLUE_API FRapidLibLinkerDLL
	{
		typedef void* (*_createModel)(); // Declare a method to store the DLL method createRegressionModel.
		_createModel m_createRegressionModelFromDLL;
		//REGRESSION
		/*typedef void* (*_createRegressionModel)(); // Declare a method to store the DLL method createRegressionModel.
		static _createRegressionModel m_createRegressionModelFromDLL;

		typedef bool (*_trainRegression) (void*, void*);
		static _trainRegression m_trainRegressionFromDLL;

		//CLASSIFICATION

		typedef void* (*_createClassificationModel)(); // Declare a method to store the DLL method createRegressionModel.
		static _createClassificationModel m_createClassificationModelFromDLL;

		typedef bool (*_trainClassification) (void*, void*);
		static _trainClassification m_trainClassificationFromDLL;

		//DTW

		typedef void* (*_createSeriesClassificationModel)(); // Declare a method to store the DLL method createRegressionModel.
		static _createSeriesClassificationModel m_createSeriesClassificationModelFromDLL;

		typedef bool (*_trainSeriesClassification) (void*, void*);// train DTW
		static _trainSeriesClassification m_trainDTWFromDLL;

		typedef bool (*_resetSeriesClassification) (void*); // reset dtw model
		static _resetSeriesClassification m_resetDTW;

		typedef void (*_destroySeriesClassificationModel)(void*); // destroy dtw model
		static _destroySeriesClassificationModel m_destroyDTW;

		typedef void* (*_runSeriesClassification)(void*, void*); // run dtw model
		static _runSeriesClassification _runDTW;

		typedef int* (*_getSeriesClassificationCosts)(void*, double* output, int numOutputs); // calculate costs of dtw model
		static _getSeriesClassificationCosts _dtwCost;


		// GENERIC

		typedef int* (*_process)(void*, double* input, int numInputs, double* output, int numOutputs);
		static _process _processModel;

		typedef const char* (*_getJSON)(void*); // Declare a method to store the DLL method createRegressionModel.
		static _getJSON m_getJSONFromDLL;
		
		typedef const void (*_putJSON)(void*, std::string jsonString); // Declare a method to store the DLL method createRegressionModel.
		static _putJSON m_putJSONDLL;

		typedef void (*_destroyModel)(void*); // Declare a method to store the DLL method createRegressionModel.
		static _destroyModel m_destroyModelFromDLL;

		// TRAINING  EXAMPLES

		typedef void* (*_createTrainingSet)();
		static _createTrainingSet m_createTrainingSetFromDLL;

		typedef void (*_destroyTrainingSet)(void*);
		static _destroyTrainingSet m_destroyTrainingSetFromDLL;

		typedef void (*_addTrainingExample)(void*, double* inputs, int numInputs, double* outputs, int numOutputs);
		static _addTrainingExample m_addTrainingExampleFromDLL;
		
		typedef int* (*_getNumTrainingExamples)(void*);
		static _getNumTrainingExamples m_getNumTrainingExamples;

		typedef double* (*_getInput)(void*, int i, int j);
		static _getInput m_getInput;

		typedef double* (*_getOutput)(void*, int i, int j);
		static _getOutput m_getOutput;

		// TRAINING SERIES 

		typedef void* (*_createTrainingSeries)();
		static _createTrainingSeries m_createTrainingSeriesFromDLL;
		
		typedef void (*_destroyTrainingSeries)(void*);
		static _destroyTrainingSeries m_destroyTrainingSeries;

		typedef void* (*_addInputsToSeries)(void*, double* inputs, int numInputs);
		static _addInputsToSeries m_addInputsToSeries;

		typedef void* (*_addLabelToSeries)(void*, std::string labelString);
		static _addLabelToSeries m_addLabelToSeries;

		typedef void* (*_createTrainingSeriesCollection)();
		static _createTrainingSeriesCollection m_createTrainingSeriesCollection;

		typedef void(*_destroyTrainingSeriesCollection)(void*);
		static _destroyTrainingSeriesCollection m_destroyTrainingSeriesCollection;

		typedef void(*_addSeriesToSeriesCollection)(void*, void*);
		static _addSeriesToSeriesCollection m_addSeriesToSeriesCollection;

		//RUN 

		typedef int (*_run)(void*, double* inputs, int numInputs, double* outputs, int numOutputs);
		static _run m_runFromDLL;*/

	public:
		FRapidLibLinkerDLL();
		static bool importDLL(FString folder, FString name);
		static int64 createRegressionModel();
		static int64 createClassificationModel();
		static FString getJSON(int64 m);
		static void destroyModel(int64 m);
		static void freeDLL();
	};

}

    

