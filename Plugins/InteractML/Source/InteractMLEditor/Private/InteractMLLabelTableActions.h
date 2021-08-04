//----
// InteractML - University Of Arts London
//----

#pragma once

#include "AssetTypeActions_Base.h"


////////////////////////////// LABEL TABLE /////////////////////////////

class FInteractMLLabelTableActions : public FAssetTypeActions_Base
{
	uint32 RegisteredCategoryHandle;
public:
	
	FInteractMLLabelTableActions(uint32 InAssetCategory)
		: RegisteredCategoryHandle(InAssetCategory)
	{
		SetSupported(true);
	}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	//virtual void GetActions(const TArray<UObject*>& InObjects, struct FToolMenuSection& Section) override;
	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions
	
};

