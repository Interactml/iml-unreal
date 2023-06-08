//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

#include "AssetTypeActions_Base.h"
#include "Misc/EngineVersionComparison.h"

////////////////////////////// LABEL /////////////////////////////


// expose our label asset type to various editor systems
//
class FInteractMLLabelActions : public FAssetTypeActions_Base
{
	uint32 RegisteredCategoryHandle;
public:
	
	FInteractMLLabelActions(uint32 InAssetCategory)
		: RegisteredCategoryHandle(InAssetCategory)
	{
#if UE_VERSION_OLDER_THAN(5,2,0) //nolonger needed, something somthing IAssetTools::IsAssetClassSupported
		SetSupported( true );
#endif
	}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions
	
};

