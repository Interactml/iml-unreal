//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

#include "AssetTypeActions_Base.h"


////////////////////////////// TRAINING SET /////////////////////////////

// expose our training set asset type to various editor systems
//
class FInteractMLTrainingSetActions : public FAssetTypeActions_Base
{
	uint32 RegisteredCategoryHandle;
public:
	
	FInteractMLTrainingSetActions(uint32 InAssetCategory)
		: RegisteredCategoryHandle(InAssetCategory)
	{
		SetSupported(true);
	}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions
	
};
