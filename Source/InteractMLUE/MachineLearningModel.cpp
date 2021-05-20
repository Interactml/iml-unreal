// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineLearningModel.h"

#include <sstream>

// Sets default values for this component's properties
UMachineLearningModel::UMachineLearningModel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


#pragma region Load DLL

// Method to import a DLL.
bool UMachineLearningModel::importDLL(FString folder, FString name)
{
    //FString filePath = *FPaths::GamePluginsDir() + folder + "/" + name;
    FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;
    
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "loading dll");


    if (FPaths::FileExists(filePath))
    {
        v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle != NULL)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "loaded dll");
            m_createRegressionModelFromDLL = NULL;
            FString procName = "createRegressionModel";    // Needs to be the exact name of the DLL method.
            m_createRegressionModelFromDLL = (_createModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_createRegressionModelFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            m_createClassificationModelFromDLL = NULL;
            procName = "createClassificationModel";    // Needs to be the exact name of the DLL method.
            m_createClassificationModelFromDLL = (_createModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_createClassificationModelFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            m_getJSONFromDLL = NULL;
            procName = "getJSON";    // Needs to be the exact name of the DLL method.
            m_getJSONFromDLL = (_getJSON)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_getJSONFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            m_destroyModelFromDLL = NULL;
            procName = "destroyModel";    // Needs to be the exact name of the DLL method.
            m_destroyModelFromDLL = (_destroyModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_destroyModelFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }


            m_createTrainingSetFromDLL = NULL;
            procName = "createTrainingSet";    // Needs to be the exact name of the DLL method.
            m_createTrainingSetFromDLL = (_createTrainingSet)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_createTrainingSetFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }


            m_destroyTrainingSetFromDLL = NULL;
            procName = "destroyTrainingSet";    // Needs to be the exact name of the DLL method.
            m_destroyTrainingSetFromDLL = (_destroyTrainingSet)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_destroyTrainingSetFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            m_addTrainingExampleFromDLL = NULL;
            procName = "addTrainingExample";    // Needs to be the exact name of the DLL method.
            m_addTrainingExampleFromDLL = (_addTrainingExample)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_addTrainingExampleFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            m_trainRegressionFromDLL = NULL;
            procName = "trainRegression";    // Needs to be the exact name of the DLL method.
            m_trainRegressionFromDLL = (_trainRegression)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_trainRegressionFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }
            
            m_trainClassificationFromDLL = NULL;
            procName = "trainClassification";    // Needs to be the exact name of the DLL method.
            m_trainClassificationFromDLL = (_trainClassification)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_trainClassificationFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            m_runFromDLL = NULL;
            procName = "process";    // Needs to be the exact name of the DLL method.
            m_runFromDLL = (_run)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_runFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            //load classification functions from dll
            m_trainDTWFromDLL = NULL;
            procName = "trainSeriesClassification";    // Needs to be the exact name of the DLL method.
            m_trainDTWFromDLL = (_trainSeriesClassification)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_trainDTWFromDLL == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw train");
                return false;
            }

            m_createDTWModelFromDLL = NULL;
            procName = "createSeriesClassificationModel";    // Needs to be the exact name of the DLL method.
            m_createDTWModelFromDLL = (_createSeriesClassificationModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_createDTWModelFromDLL == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw model function");
                return false;
            }
            
            m_resetDTW = NULL;
            procName = "resetSeriesClassification";    // Needs to be the exact name of the DLL method.
            m_resetDTW = (_resetSeriesClassification)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_resetDTW == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to reset dtw model function");
                return false;
            }
            
            m_destroyDTW = NULL;
            procName = "destroySeriesClassificationModel";    // Needs to be the exact name of the DLL method.
            m_destroyDTW = (_destroySeriesClassificationModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_destroyDTW == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to destroy dtw model function");
                return false;
            }
            
            m_runDTW = NULL;
            procName = "runSeriesClassification";    // Needs to be the exact name of the DLL method.
            m_runDTW = (_runSeriesClassification)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_runDTW == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw run function");
                return false;
            }
            
            m_dtwCost = NULL;
            procName = "getSeriesClassificationCosts";    // Needs to be the exact name of the DLL method.
            m_dtwCost = (_getSeriesClassificationCosts)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_dtwCost == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw cost function");
                return false;
            }
            
            m_dtwTrainingSeriesCollection = NULL;
            procName = "createTrainingSeriesCollection";    // Needs to be the exact name of the DLL method.
            m_dtwTrainingSeriesCollection = (_createTrainingSeriesCollection)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_dtwTrainingSeriesCollection == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw training series function");
                return false;
            }
            
            m_addToSeries = NULL;
            procName = "addInputsToSeries";    // Needs to be the exact name of the DLL method.
            m_addToSeries = (_addInputsToSeries)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_addToSeries == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw input to series");
                return false;
            }
            
            m_addLabelDTW = NULL;
            procName = "addLabelToSeries";    // Needs to be the exact name of the DLL method.
            m_addLabelDTW = (_addLabelToSeries)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_addLabelDTW == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw label to series");
                return false;
            }
            
            m_dtwTrainingSeries = NULL;
            procName = "createTrainingSeries";    // Needs to be the exact name of the DLL method.
            m_dtwTrainingSeries = (_createTrainingSeries)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_dtwTrainingSeries == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw create series");
                return false;
            }
            
            m_addSeriesToDTWCollection = NULL;
            procName = "addSeriesToSeriesCollection";    // Needs to be the exact name of the DLL method.
            m_addSeriesToDTWCollection = (_addSeriesToSeriesCollection)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_addSeriesToDTWCollection == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw create series");
                return false;
            }
            
            m_trainDTWFromDLL = NULL;
            procName = "trainSeriesClassification";    // Needs to be the exact name of the DLL method.
            m_trainDTWFromDLL = (_trainSeriesClassification)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_trainDTWFromDLL == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw train series");
                return false;
            }
            
            
            m_destroyDTWDLLTrainingSet = NULL;
            procName = "destroyTrainingSeriesCollection";    // Needs to be the exact name of the DLL method.
            m_destroyDTWDLLTrainingSet = (_destroyTrainingSeriesCollection)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_destroyDTWDLLTrainingSet == NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dtw destroy training set");
                return false;
            }

            //std::stringstream ss;
            //ss << v_dllHandle;

            //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("loaded dll ") + ss.str().c_str());
            return true;
        }
        //else {
        //    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dll");
        //}
    }
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "could not find path");
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, filePath);
    return false;    // Return an error.
}

bool  UMachineLearningModel::IsDLLLoaded()
{
    std::stringstream ss;
    ss << v_dllHandle;

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("dll pointer ") + ss.str().c_str());

    return (v_dllHandle != NULL);
}

#pragma endregion Load DLL



#pragma region Unload DLL

// If you love something  set it free.
void UMachineLearningModel::freeDLL()
{
    if (v_dllHandle != NULL)
    {
        m_createRegressionModelFromDLL = NULL;
        m_createClassificationModelFromDLL = NULL;

        FPlatformProcess::FreeDllHandle(v_dllHandle);
        v_dllHandle = NULL;
    }
}
#pragma endregion Unload DLL


#pragma region Machine Learning

bool UMachineLearningModel::addTrainingData(TArray<float> input, TArray<float> output) {
    FDataInstance data;
    data.inputs = input;
    data.outputs = output;

    if (m_numInputs < 0) {
        m_numInputs = input.Num();
    }
    else {
        if (m_numInputs != input.Num()) {
            return false;
        }
    }

    if (m_numOutputs < 0) {
        m_numOutputs = output.Num();
    }
    else {
        if (m_numOutputs != output.Num()) {
            return false;
        }
    }

    m_dataset.Add(data);
    return true;
}

bool UMachineLearningModel::stopCollecting() {
    if (collecting) {
        collecting = false;
        if (m_datasetSeriesSingle.inputSeries.Num() > 0 && !m_datasetSeriesSingle.label.IsEmpty()) {
            m_datasetSeries.Add(m_datasetSeriesSingle);
            return true;
        }
    }
    return collecting;
}

bool UMachineLearningModel::addTrainingDataSeries(FDataInstanceSeriesMember trainingSeriesMember, FString label) {
    FDataInstanceSeriesMember data;
    data = trainingSeriesMember;
    // if this is the beginning of collecting data
    if (!collecting) {
        collecting = true;
        // clear the previous dtw series 
        m_datasetSeriesSingle.inputSeries.Empty();
        // check if the label is empty 
        if (!label.IsEmpty()) {
            m_datasetSeriesSingle.label = label;
        }
        else {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "no label");
            return false;
        }
            
        
            
    }
    if (data.inputData.Num() == m_numInputs) {
        m_datasetSeriesSingle.inputSeries.Add(data);
        return true;
    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(m_numInputs));
    }
    return false;
}

bool UMachineLearningModel::SetUpInputsOutputs(FDataInstance example) {
    bool setInputsOutputs = false;
    m_numInputs = example.inputs.Num();
    m_numOutputs = example.outputs.Num();
    if (m_numInputs > 0 && m_numOutputs > 0) {
        setInputsOutputs = true;
    }
    return setInputsOutputs;
}

FDataInstanceSeriesMember UMachineLearningModel::SetUpSeriesStruct(TArray<float> inputs) {
    m_datasetSeriesSingleMember.inputData = inputs;
     
    return m_datasetSeriesSingleMember;
}

bool UMachineLearningModel::SetUpInputsOutputsSerie(FDataInstanceSeriesMember example, FString label) {
    bool setInputsOutputs = false;

    m_numInputs = example.inputData.Num();
    if (m_numInputs > 0) {
        setInputsOutputs = true;
    }
    return setInputsOutputs;
}

bool  UMachineLearningModel::IsModelInitialised()
{
    return (m_modelPtr != NULL);
}

bool UMachineLearningModel::trainRegressor() {

    if(m_modelPtr == NULL)
    {
        if (m_createRegressionModelFromDLL != NULL)
        {
            m_modelPtr = m_createRegressionModelFromDLL();
            if (m_modelPtr == NULL) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    void* trainingSet = m_createTrainingSetFromDLL();

    for (int32 i = 0; i < m_dataset.Num(); i++) {
        FDataInstance d = m_dataset[i];

        // copy data into plain c arrays
        int j;
        double *in = new double[d.inputs.Num()];
        for (j = 0; j < d.inputs.Num(); j++) {
            in[j] = d.inputs[j];
        }
        double* out = new double[d.outputs.Num()];
        for (j = 0; j < d.outputs.Num(); j++) {
            out[j] = d.outputs[j];
        }

        // use these to add to the training data
        m_addTrainingExampleFromDLL(trainingSet, in, d.inputs.Num(), out, d.outputs.Num());
    }

    bool result = m_trainRegressionFromDLL(m_modelPtr, trainingSet);

    m_destroyTrainingSetFromDLL(trainingSet);
    
    return result;
}

bool UMachineLearningModel::trainClassifier() {

    if (m_modelPtr == NULL)
    {
        if (m_createClassificationModelFromDLL != NULL)
        {
            m_modelPtr = m_createClassificationModelFromDLL();
            if (m_modelPtr == NULL) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    void* trainingSet = m_createTrainingSetFromDLL();

    for (int32 i = 0; i < m_dataset.Num(); i++) {
        FDataInstance d = m_dataset[i];

        // copy data into plain c arrays
        int j;
        double* in = new double[d.inputs.Num()];
        for (j = 0; j < d.inputs.Num(); j++) {
            in[j] = d.inputs[j];
        }
        double* out = new double[d.outputs.Num()];
        for (j = 0; j < d.outputs.Num(); j++) {
            out[j] = d.outputs[j];
        }

        // use these to add to the training data
        m_addTrainingExampleFromDLL(trainingSet, in, d.inputs.Num(), out, d.outputs.Num());
    }

    // train DTW 
    bool result = m_trainClassificationFromDLL(m_modelPtr, trainingSet);

    // destroy training set
    m_destroyTrainingSetFromDLL(trainingSet);

    return result;
}

/// <summary>
/// trains dynamic time warping
/// </summary>
/// <returns></returns>
bool UMachineLearningModel::trainDTW() {
    // check if there is training data
    if (m_datasetSeries.Num() == 0) {

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "no training data for dtw");
        return false;
    }

    // check is model is null
    if (m_modelPtr == NULL)
    {
        // check if method from dll is null 
        if (m_createDTWModelFromDLL != NULL)
        {
            m_modelPtr = m_createDTWModelFromDLL();
            if (m_modelPtr == NULL) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    // reset model
    m_resetDTW(m_modelPtr);

    // create dtw training set
    void* trainingSeriesCollectionTemp = m_dtwTrainingSeriesCollection();

    
    for(FDataInstanceSeries dtwExample : m_datasetSeries) {
        void* trainingSeriesTemp = m_dtwTrainingSeries();

        for (FDataInstanceSeriesMember seriesItem : dtwExample.inputSeries) {
            // copy data into plain c arrays
            int j;
            double* in = new double[seriesItem.inputData.Num()];
            for (j = 0; j < seriesItem.inputData.Num(); j++) {
                in[j] = seriesItem.inputData[j];
            }
            m_addToSeries(trainingSeriesTemp, in, seriesItem.inputData.Num());
        }

        // label the series
        m_addLabelDTW(trainingSeriesTemp, *dtwExample.label);

        // add series to collection 
        m_addSeriesToDTWCollection(trainingSeriesCollectionTemp, trainingSeriesTemp);
    }
    // train ghe model 
    bool result = m_trainDTWFromDLL(m_modelPtr, trainingSeriesCollectionTemp);
    
    //bool result = m_trainClassificationFromDLL(m_modelPtr, trainingSet);

    //m_destroyTrainingSetFromDLL(trainingSet);
    return result;
}

TArray<float> UMachineLearningModel::Run(TArray<float> input)
{
    TArray<float> result;
    if (m_modelPtr != NULL)
    {
        // convert the input to a plain array
        int i;
        double* in = new double[input.Num()];
        for (i = 0; i < input.Num(); i++) {
            in[i] = input[i];
        }

        // create a plain array to hold the result
        double* out = new double[m_numOutputs];

        int numOut = m_runFromDLL(m_modelPtr, in, input.Num(), out, m_numOutputs);
        
        for (i = 0; i < numOut; i++) {
            result.Add(out[i]);
        }
    }
    return result;
}


#pragma endregion Machine Learning



// Called when the game starts
void UMachineLearningModel::BeginPlay()
{
	Super::BeginPlay();

//#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "beginning play");
//#endif
    if (!importDLL(m_dllFolder, m_dllName))
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "free dll");
        freeDLL(); // if the import has failed, clean up
    }
	
}



// Called when the game starts
void UMachineLearningModel::BeginDestroy()
{
    Super::BeginDestroy();

    freeDLL(); 

}



// Called every frame
void UMachineLearningModel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



