// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractML.h"

/*
typedef bool(*_getInvertedBool)(bool boolState); // Declare a method to store the DLL method getInvertedBool.
typedef int(*_getIntPlusPlus)(int lastInt); // Declare a method to store the DLL method getIntPlusPlus.
typedef float(*_getCircleArea)(float radius); // Declare a method to store the DLL method getCircleArea.
typedef char* (*_getCharArray)(char* parameterText); // Declare a method to store the DLL method getCharArray.
typedef float* (*_getVector4)(float x, float y, float z, float w); // Declare a method to store the DLL method getVector4.

_getInvertedBool m_getInvertedBoolFromDll;
_getIntPlusPlus m_getIntPlusPlusFromDll;
_getCircleArea m_getCircleAreaFromDll;
_getCharArray m_getCharArrayFromDll;
_getVector4 m_getVector4FromDll;
*/


typedef void *(*_createModel)(); // Declare a method to store the DLL method createRegressionModel.
_createModel m_createRegressionModelFromDLL;

typedef void *(*_createModel)(); // Declare a method to store the DLL method createRegressionModel.
_createModel m_createClassificationModelFromDLL;

typedef const char* (*_getJSON)(void*); // Declare a method to store the DLL method createRegressionModel.
_getJSON m_getJSONFromDLL;

typedef void (*_destroyModel)(void *); // Declare a method to store the DLL method createRegressionModel.
_destroyModel m_destroyModelFromDLL;

int64 modelAddress; 


void* v_dllHandle;


#pragma region Load DLL

// Method to import a DLL.
bool UInteractML::importDLL(FString folder, FString name)
{
    FString filePath = *FPaths::GameDevelopersDir() + folder + "/" + name;

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
                return false;
            }
            
            m_createClassificationModelFromDLL = NULL;
            procName = "createClassificationModel";    // Needs to be the exact name of the DLL method.
            m_createClassificationModelFromDLL = (_createModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_createClassificationModelFromDLL == NULL)
            {
                return false;
            }

            m_getJSONFromDLL = NULL;
            procName = "getJSON";    // Needs to be the exact name of the DLL method.
            m_getJSONFromDLL = (_getJSON)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_getJSONFromDLL == NULL)
            {
                return false;
            }

            m_destroyModelFromDLL = NULL;
            procName = "destroyModel";    // Needs to be the exact name of the DLL method.
            m_destroyModelFromDLL = (_destroyModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            if (m_destroyModelFromDLL == NULL)
            {
                return false;
            }

            return true;
        }
    }
    return false;    // Return an error.
}
#pragma endregion Load DLL

#pragma region Method Calls


// Calls the method m_getVector4FromDll that was imported from the DLL.
int64 UInteractML::createRegressionModel()
{
    if (m_createRegressionModelFromDLL != NULL)
    {
        void * v_classPtr = m_createRegressionModelFromDLL();
        return (int64)v_classPtr;
    }
    return 0;
}

// Calls the method m_getVector4FromDll that was imported from the DLL.
int64 UInteractML::createClassificationModel()
{
    if (m_createClassificationModelFromDLL != NULL)
    {
        void * v_classPtr = m_createClassificationModelFromDLL();
        return (int64)v_classPtr;
    }
    return 0;
}

// Calls the method m_getVector4FromDll that was imported from the DLL.
FString UInteractML::getJSON(int64 m)
{
    if (m_getJSONFromDLL != NULL)
    {
        const char* json = m_getJSONFromDLL((void *)m);

        return (ANSI_TO_TCHAR(json));
    }
    return "Error: failed to load JSON, maybe the DLL was not loaded yet";
}


// Calls the method m_getVector4FromDll that was imported from the DLL.
 void UInteractML::destroyModel(int64 m)
{
    if (m != NULL && m_destroyModelFromDLL != NULL)
    {
        m_destroyModelFromDLL((void *) m);
    }
}


#pragma endregion Method Calls


#pragma region Unload DLL

// If you love something  set it free.
void UInteractML::freeDLL()
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
