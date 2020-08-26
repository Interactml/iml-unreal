// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
static class INTERACTMLUE_API DataSerialization
{
public:

	static void SaveFile(FString Content, FString Directory, FString FileName);
	//implemented for DTW
	static void ParseIMLFeatureToJSON();
	static void SaveModelToDisk(FString filename, int64 modelAddress);
	static FString ReadFile(FString Directory, FString FileName);
};
