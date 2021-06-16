//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal

//module
#include "InteractMLModelNode.h"
#include "InteractMLDynamicTimeWarpNode.generated.h"

//general declarations

// Classification blueprint node customisations
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLDynamicTimeWarpNode: public UInteractMLModelNode
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