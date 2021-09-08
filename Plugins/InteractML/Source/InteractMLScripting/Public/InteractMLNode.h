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
class INTERACTMLSCRIPTING_API UInteractMLNode: public UK2Node
{
	GENERATED_BODY()

public:
	//Allow the operating context of the node to be explicitly specified instead of defaulting to the containing graph
	UPROPERTY( EditAnywhere, AdvancedDisplay )
	bool bCustomContextActor;

private:

	
protected:

	//~ Begin UObject Interface
	//virtual void Serialize(FArchive& Ar) override;	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	//virtual void PostLoad() override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	//virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const override;
	//virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	//virtual bool IncludeParentNodeContextMenu() const override { return true; }
	//virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	//virtual void PostPlacedNewNode();
	virtual FLinearColor GetNodeTitleColor() const override;
	//~ End UEdGraphNode Interface

	//~ Begin K2Node Interface
	//virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	//virtual bool IsNodePure() const override { return bIsPureGet; }
	//virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual bool ShouldShowNodeProperties() const override { return true; } //now has config properties
	//virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	//virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	//virtual void PostReconstructNode();
	//~ End K2Node Interface};

	//actor context hookup
	void ConnectContextActor(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph, UEdGraphPin* ActorRecipientPin);
	UEdGraphPin* GetActorInputPin() const;
	
	
private:

	UFunction* FindModelTrainFunction() const;
	
};