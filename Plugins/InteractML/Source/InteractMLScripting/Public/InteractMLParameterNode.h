//----
// InteractML - University Of Arts London
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
//#include "InteractMLParameters.h"
#include "InteractMLParameterNode.generated.h"


//general declarations

// we track and store the types of inputs attached to this node as parameter sources
// TODO: later we can enable renaming/editing of this list explicitly instead of just dragging onto wild nodes
//
USTRUCT()
struct FParameterSpec
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;
	
	UPROPERTY()
	FName Type;

	//more needed for certain struct based types, e.g. FVector


	//setup
	FParameterSpec(){}
	FParameterSpec(FName type, FString name) : Name(name), Type(type) {}
};


//main class

// Parameter collection custom blueprint node
// Allows any number of inputs, of various types, to be connected to be accumulated into a single UInteractMLParameters 
// collection object for passing around the graph and consumption by training and model nodes.
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLParameterNode: public UK2Node, public IK2Node_AddPinInterface
{
	GENERATED_BODY()
	
	// parameters known to be expected as inputs to this node
	UPROPERTY()
	TArray<FParameterSpec> InputParameters;

public:

private:

	//specialist	
	//UEdGraphPin* GetOutputPin() const;	

protected:


	//~ Begin UObject Interface
	//virtual void Serialize(FArchive& Ar) override;	
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	//virtual void PostLoad() override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
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
	virtual bool ShouldShowNodeProperties() const override { return false; }
	//virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//~ End K2Node Interface};

	// IK2Node_AddPinInterface interface
	virtual void AddInputPin() override;
	// End of IK2Node_AddPinInterface interface

private:
	int CountParameters() const; //only valid ones
	int CountFloats() const; //only valid ones
};