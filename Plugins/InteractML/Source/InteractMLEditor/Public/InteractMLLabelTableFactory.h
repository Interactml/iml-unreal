//----
// InteractML - University Of Arts London
//----

#pragma once

// unreal
#include "Editor.h"
#include "Factories/DataTableFactory.h"

// apparance

// module

// auto (last)
#include "InteractMLLabelTableFactory.generated.h"


////////////////////////////// LABEL /////////////////////////////

// makes a label table asset (based on built in DataTable asset type)
//
UCLASS()
class INTERACTMLEDITOR_API UInteractMLLabelTableFactory
	: public UDataTableFactory
{
	GENERATED_UCLASS_BODY()

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	//~ Begin UFactory Interface
	
protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};
