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
#include "InteractMLExternalTrainingSetNode.generated.h"


//general declarations

// Direct access to external training set data as a temporary training set object
// Needed when you have new training sets created or added at runtime (no asset changes possible)
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLExternalTrainingSetNode : public UInteractMLNode
{
	GENERATED_BODY()

public:

private:

	//specialist
	UEdGraphPin* GetDataPathInputPin() const;
	UEdGraphPin* GetTrainingSetOutputPin() const;
	UEdGraphPin* GetHasDataOutputPin() const;
	
protected:

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	//~ End UEdGraphNode Interface

	//~ Begin K2Node Interface
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	//~ End K2Node Interface
	
private:

	UFunction* FindTrainingSetFunction() const;
	
};