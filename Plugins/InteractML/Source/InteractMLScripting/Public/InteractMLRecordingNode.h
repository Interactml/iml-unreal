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
#include "InteractMLRecordingNode.generated.h"


//general declarations

UENUM()
enum class EInteractMLRecordingMode
{
	Single,
	Series,
};


// Recording of training set data
// Record single/series of live parameter samples into a training set object
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLRecordingNode : public UInteractMLNode
{
	GENERATED_BODY()

public:
	//single or series sample recording?
	UPROPERTY(EditAnywhere, Category=InteracML,meta=(Tooltip="Record a single value each time activated or a continuous series whilst active?"))
	EInteractMLRecordingMode Mode;

	//label type (expected outputs)
	UPROPERTY(EditAnywhere, Category=InteracML,meta=(HideViewOptions,ShowDisplayNames,Tooltip="Specify composite label type by selecting a Label Type asset that defines the output parameters to train against, leave None for simple numerical output"))
	UInteractMLLabel* LabelType;

private:

	//specialist
	UEdGraphPin* GetTrainingSetInputPin() const;
	UEdGraphPin* GetLiveParametersInputPin() const;
	UEdGraphPin* GetLabelInputPin() const;
	UEdGraphPin* GetRecordInputPin() const;
	UEdGraphPin* GetDeleteLastInputPin() const;
	UEdGraphPin* GetDeleteLabelInputPin() const;
	UEdGraphPin* GetDeleteAllInputPin() const;
	UEdGraphPin* GetChangedOutputPin() const;
	
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
	virtual bool ShouldShowNodeProperties() const override { return true; } //We have configuration properties we need the user to have access to
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	//~ End K2Node Interface

private:

	UFunction* FindRecordFunction() const;
	
};