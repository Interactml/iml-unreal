// Fill out your copyright notice in the Description page of Project Settings.


#include "DataSerialization.h"



void DataSerialization::ParseIMLFeatureToJSON() {

    //implemented when DTW gets done 
 
}
//Save text file 
void  DataSerialization::SaveFile(FString Content, FString Directory, FString FileName)
{
    Directory = *FPaths::ProjectContentDir() + Directory + "/" + FileName;
    if (FFileHelper::SaveStringToFile(Content, *Directory, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append)) {
        UE_LOG(LogTemp, Warning, TEXT("Hello World!"))
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Success!"));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Fail!"))
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fail!"));
    }

}
FString DataSerialization::ReadFile(FString Directory, FString FileName)
{
    const FString JsonFilePath = *FPaths::ProjectContentDir() + Directory + "/" + FileName;
    FString JsonString; //Json converted to FString
    if (FFileHelper::LoadFileToString(JsonString, *JsonFilePath)) {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "yes");
    }

    return JsonString;
}

void DataSerialization::DeleteFile(FString Directory, FString FileName)
{
    const FString JsonFilePath = *FPaths::ProjectContentDir() + Directory + "/" + FileName;

    if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*JsonFilePath))
    {
        return;
    }
}
void SaveModelToDisk(FString fileName, int64 modelAddress) {
    
    //FString m_ModelJSONString = RapidlibLinkerDLL.GetJSON(m_ModelAddress);

     // Saves the model as a stylised json with the specified fileName
    //IMLDataSerialization.SaveRapidlibModelToDisk(m_ModelJSONString, fileName);

 }


