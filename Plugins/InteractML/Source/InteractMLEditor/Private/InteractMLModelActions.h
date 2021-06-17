//----
// InteractML - University Of Arts London
//----

#pragma once

#include "AssetTypeActions_Base.h"


////////////////////////////// CLASSIFICATION MODEL /////////////////////////////

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
//	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions
	
};


////////////////////////////// REGRESSION MODEL /////////////////////////////

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
	//	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions
	
};


////////////////////////////// DTW MODEL /////////////////////////////

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
	//	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions
	
};
