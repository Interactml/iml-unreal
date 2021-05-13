// Fill out your copyright notice in the Description page of Project Settings.
//Links to RapidLib dll exposing its functionality to c#. Doesn't hold data


#include "RapidLibLinkerDLL.h"


int64 modelAddress; 




void* v_dllHandle;




namespace InteractML {

    FRapidLibLinkerDLL::FRapidLibLinkerDLL()
    {

    }

#pragma region Load DLL

    bool FRapidLibLinkerDLL::importDLL(FString folder, FString name)
    {
        FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "loading dll");


        if (FPaths::FileExists(filePath))
        {
            v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
            if (v_dllHandle != NULL)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "loaded dll");
                /*m_createRegressionModelFromDLL = NULL;
                FString procName = "createRegressionModel";    // Needs to be the exact name of the DLL method.
                m_createRegressionModelFromDLL = (_createModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
                if (m_createRegressionModelFromDLL == NULL)
                {
                    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "failed to load function");
                    return false;
                }*/

                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("loaded dll ") + ss.str().c_str());
                return true;
            }
            
        }
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "could not find path");
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, filePath);
        return false;    // Return an error.
    }
    // Method to import a DLL.
   /* bool FRapidLibLinkerDLL::importDLL(FString folder, FString name)
    {
        FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;

        if (FPaths::FileExists(filePath))
        {
            v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
            if (v_dllHandle != NULL)
            {
                m_createRegressionModelFromDLL = NULL;
                FString procName = "createRegressionModel";    // Needs to be the exact name of the DLL method.
                m_createRegressionModelFromDLL = (_createRegressionModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
                if (m_createRegressionModelFromDLL == NULL)
                {
                    return false;
                }

                m_createClassificationModelFromDLL = NULL;
                procName = "createClassificationModel";    // Needs to be the exact name of the DLL method.
                m_createClassificationModelFromDLL = (_createClassificationModel)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
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
    }*/
#pragma endregion Load DLL

#pragma region Method Calls


    // Calls the method m_getVector4FromDll that was imported from the DLL.
    int64 FRapidLibLinkerDLL::createRegressionModel()
    {
        /*if (m_createRegressionModelFromDLL != NULL)
        {
            void* v_classPtr = m_createRegressionModelFromDLL();
            return (int64)v_classPtr;
        }*/
        return 0;
    }

    // Calls the method m_getVector4FromDll that was imported from the DLL.
    int64 FRapidLibLinkerDLL::createClassificationModel()
    {
        /*if (m_createClassificationModelFromDLL != NULL)
        {
            void* v_classPtr = m_createClassificationModelFromDLL();
            return (int64)v_classPtr;
        }*/
        return 0;
    }

    // Calls the method m_getVector4FromDll that was imported from the DLL.
    FString FRapidLibLinkerDLL::getJSON(int64 m)
    {
        /*if (m_getJSONFromDLL != NULL)
        {
            const char* json = m_getJSONFromDLL((void*)m);

            return (ANSI_TO_TCHAR(json));
        }*/
        return "Error: failed to load JSON, maybe the DLL was not loaded yet";
    }


    // Calls the method m_getVector4FromDll that was imported from the DLL.
    void FRapidLibLinkerDLL::destroyModel(int64 m)
    {
       /* if (m != NULL && m_destroyModelFromDLL != NULL)
        {
            m_destroyModelFromDLL((void*)m);
        }*/
    }


#pragma endregion Method Calls


#pragma region Unload DLL

    // If you love something  set it free.
    void FRapidLibLinkerDLL::freeDLL()
    {
        if (v_dllHandle != NULL)
        {
           // m_createRegressionModelFromDLL = NULL;
            //m_createClassificationModelFromDLL = NULL;

            FPlatformProcess::FreeDllHandle(v_dllHandle);
            v_dllHandle = NULL;
        }
    }

}


#pragma endregion Unload DLL
