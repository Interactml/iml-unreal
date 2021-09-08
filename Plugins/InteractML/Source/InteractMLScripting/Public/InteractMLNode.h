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
//#include "InteractML.h"
#include "InteractMLNode.generated.h"


//general declarations

// Base for all InteractML nodes, there is a little common functionality and shared utility
//
UCLASS(Abstract)
class INTERACTMLSCRIPTING_API UInteractMLNode : public UK2Node
{
	GENERATED_BODY()

public:
	//Allow the operating context of the node to be explicitly specified instead of defaulting to the containing graph
	UPROPERTY( EditAnywhere, AdvancedDisplay, Category=InteracML )
	bool bCustomContextActor;

private:

protected:

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetMenuCategory() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FLinearColor GetNodeBodyTintColor() const override;
	virtual FSlateIcon GetIconAndTint( FLinearColor& OutColor ) const override;
	//~ End UEdGraphNode Interface

	//~ Begin K2Node Interface
	virtual bool ShouldShowNodeProperties() const override { return true; } //now has config properties
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//~ End K2Node Interface

	//actor context hookup
	void ConnectContextActor(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph, UEdGraphPin* ActorRecipientPin);
	UEdGraphPin* GetActorInputPin() const;
	
private:

	UFunction* FindModelTrainFunction() const;
	
};