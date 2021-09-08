//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

#include "AssetTypeActions_Base.h"


////////////////////////////// CLASSIFICATION MODEL /////////////////////////////

// expose our classification model asset type to various editor systems
//
class FInteractMLClassificationModelActions : public FAssetTypeActions_Base
{
	uint32 RegisteredCategoryHandle;
public:
	
	FInteractMLClassificationModelActions(uint32 InAssetCategory)
		: RegisteredCategoryHandle(InAssetCategory)
	{
		SetSupported(true);
	}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	// End IAssetTypeActions
	
};


////////////////////////////// REGRESSION MODEL /////////////////////////////

// expose our regression model asset type to various editor systems
//
class FInteractMLRegressionModelActions : public FAssetTypeActions_Base
{
	uint32 RegisteredCategoryHandle;
public:
	
	FInteractMLRegressionModelActions(uint32 InAssetCategory)
	: RegisteredCategoryHandle(InAssetCategory)
	{
		SetSupported(true);
	}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	// End IAssetTypeActions
	
};


////////////////////////////// DTW MODEL /////////////////////////////

// expose our DTW model asset type to various editor systems
//
class FInteractMLDynamicTimewarpModelActions : public FAssetTypeActions_Base
{
	uint32 RegisteredCategoryHandle;
public:
	
	FInteractMLDynamicTimewarpModelActions(uint32 InAssetCategory)
	: RegisteredCategoryHandle(InAssetCategory)
	{
		SetSupported(true);
	}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	// End IAssetTypeActions
	
};
