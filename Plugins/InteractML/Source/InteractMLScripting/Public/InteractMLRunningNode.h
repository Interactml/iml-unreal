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
#include "InteractMLLabel.h"
#include "InteractMLRunningNode.generated.h"


//general declarations

// Model running blueprint node for using a model to perform recognition
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLRunningNode: public UInteractMLNode
{
	GENERATED_BODY()

public:
	//label type (expected outputs)
	UPROPERTY(EditAnywhere, Category=InteracML,meta=(Tooltip="Specify composite label type by selecting a Label Type asset that defines the output parameters expected, leave None for simple numerical output"))
	UInteractMLLabel* LabelType;

	UPROPERTY(EditAnywhere, Category=InteracML,meta=(Tooltip="Enable to run this node in the background instead of blocking blueprint operation"))
	bool bBackgroundOperation;

private:

	//specialist
	UEdGraphPin* GetModelInputPin() const;
	UEdGraphPin* GetLiveParametersInputPin() const;
	UEdGraphPin* GetRunInputPin() const;
	UEdGraphPin* GetLabelOutputPin() const;
	UEdGraphPin* GetRunningOutputPin() const;
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

	UFunction* FindModelRunFunction() const;
	
};