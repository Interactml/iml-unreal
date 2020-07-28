// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractML.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTMLUE_API UInteractML : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importDLL(FString folder, FString name);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static int64 createRegressionModel();
    
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static int64 createClassificationModel();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static FString getJSON(int64 m);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void destroyModel(int64 m);


    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void freeDLL();
};
