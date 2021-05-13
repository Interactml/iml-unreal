// Fill out your copyright notice in the Description page of Project Settings.


#include "RapidLibLinkerTemp.h"

#include <sstream>

// Sets default values for this component's properties
FRapidLibLinkerDLLTemp::FRapidLibLinkerDLLTemp()
{
	
}


#pragma region Load DLL

// Method to import a DLL.
bool FRapidLibLinkerDLLTemp::importDLL(FString folder, FString name)
{
    // find the dll 
    FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;

    // if the file path exists 
    if (FPaths::FileExists(filePath))
    {
        v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle != NULL)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "loaded dll");
            m_createRegressionModelFromDLL = NULL;
            FString procName = "createRegressionModel";    // Needs to be the exact name of the DLL method.
            m_createRegressionModelFromDLL = (_createRegressionModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_createRegressionModelFromDLL == NULL)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                return false;
            }

            m_createClassificationModelFromDLL = NULL;
            procName = "createClassificationModel";    // Needs to be the exact name of the DLL method.
            m_createClassificationModelFromDLL = (_createClassificationModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
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

            std::stringstream ss;
            ss << v_dllHandle;

            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("loaded dll ") + ss.str().c_str());
            return true;
        }
        else {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load dll");
        }
    }
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "could not find path");
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, filePath);
    return false;    // Return an error.
}



#pragma endregion Load DLL














