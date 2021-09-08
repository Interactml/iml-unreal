//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Engine/DataTable.h"

//module
#include "InteractMLHelpers.h"
#include "InteractMLLabelTable.generated.h"

//general declarations

// InteractML Label Table
// Collection of label instances
// Used to define a set of label values for training purposes
// NOTE: Just a simple specialisation of Data Table asset type, we want all the functionality/features but don't really need to add much
//
UCLASS(hidecategories=Object, MinimalAPI, BlueprintType)
class UInteractMLLabelTable : public UDataTable
{
	GENERATED_BODY()
	
	/////////// DERIVED/NON-PERSISTENT ///////////

public:

private:
	
	///////////// INTERNALS //////////////

};
