//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal

//module
#include "InteractMLModelNode.h"
#include "InteractMLRegressionNode.generated.h"

//general declarations

// Classification blueprint node customisations
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLRegressionNode: public UInteractMLModelNode
{
	GENERATED_BODY()
		
public:
		
private:
		
protected:		

	//~ Begin model specific overrides
	virtual FText GetModelName() const override;
	virtual FText GetModelTooltip() const override;
	virtual FName GetModelAccessFunctionName() const override;
	//~ End model specific overrides
		
private:		
		
};