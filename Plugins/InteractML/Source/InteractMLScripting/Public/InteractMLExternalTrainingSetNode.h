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
#include "InteractMLNode.h"
#include "InteractMLExternalTrainingSetNode.generated.h"


//general declarations

// Direct access to external training set data as a temporary training set object
// Needed when you have new training sets created or added at runtime (no asset changes possible)
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLExternalTrainingSetNode: public UInteractMLNode
{
	GENERATED_BODY()

public:

private:

	//specialist
	UEdGraphPin* GetDataPathInputPin() const;
	UEdGraphPin* GetTrainingSetOutputPin() const;
	UEdGraphPin* GetHasDataOutputPin() const;
	
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
	//virtual FText GetMenuCategory() const override;
	//virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	//virtual bool IncludeParentNodeContextMenu() const override { return true; }
	//virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	//virtual void PostPlacedNewNode();
	//virtual FLinearColor GetNodeTitleColor() const override;
	//~ End UEdGraphNode Interface

	//~ Begin K2Node Interface
	//virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	//virtual bool IsNodePure() const override { return bIsPureGet; }
	//virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	//virtual bool ShouldShowNodeProperties() const override { return false; }
	//virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	//virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	//virtual void PostReconstructNode();
	//~ End K2Node Interface};


	
private:

	UFunction* FindTrainingSetFunction() const;
	
};