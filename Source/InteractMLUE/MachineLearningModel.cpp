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
    FString filePath = *FPaths::GamePluginsDir() + folder + "/" + name;

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "loading dll");


    if (FPaths::FileExists(filePath))
    {
        v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle != NULL)
        {
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

bool UMachineLearningModel::SetUpInputsOutputs(FDataInstance example) {
    bool setInputsOutputs = false;
    m_numInputs = example.inputs.Num();
    m_numOutputs = example.outputs.Num();
    if (m_numInputs > 0 && m_numOutputs > 0) {
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

    bool result = m_trainClassificationFromDLL(m_modelPtr, trainingSet);

    m_destroyTrainingSetFromDLL(trainingSet);

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



