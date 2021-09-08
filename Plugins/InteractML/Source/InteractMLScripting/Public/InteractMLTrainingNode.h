//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/Blueprint.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"

//module
#include "InteractMLNode.h"
#include "InteractMLTrainingNode.generated.h"


//general declarations

// Training blueprint node for applying training set data to a model
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLTrainingNode : public UInteractMLNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=InteracML,meta=(Tooltip="Enable to train this node in the background instead of blocking blueprint operation"))
	bool bBackgroundOperation;

private:

	//specialist
	UEdGraphPin* GetModelInputPin() const;
	UEdGraphPin* GetTrainingSetInputPin() const;
	UEdGraphPin* GetTrainInputPin() const;
	UEdGraphPin* GetResetInputPin() const;
	UEdGraphPin* GetTrainedOutputPin() const;
	UEdGraphPin* GetTrainingOutputPin() const;
	UEdGraphPin* GetCompletedOutputPin() const;
	
protected:

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	//~ End UEdGraphNode Interface

	//~ Begin K2Node Interface
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	//~ End K2Node Interface
	
private:

	UFunction* FindModelTrainFunction() const;
	
};