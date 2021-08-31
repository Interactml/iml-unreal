//----
// InteractML - University Of Arts London
//----

#pragma once

// unreal
#include "Editor.h"

// apparance

// module

// auto (last)
#include "InteractMLLabelFactory.generated.h"


////////////////////////////// LABEL /////////////////////////////

// makes a label asset
//
UCLASS()
class INTERACTMLEDITOR_API UInteractMLLabelFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags tFlags, UObject* Context, FFeedbackContext* Warn) override;
};
