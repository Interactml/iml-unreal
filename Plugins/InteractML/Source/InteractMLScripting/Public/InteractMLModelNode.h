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
//#include "InteractML.h"
#include "InteractMLModelNode.generated.h"


//general declarations

// Model blueprint node base for specific model types
// Most model functionality is shared (training, load/save and running blueprint node)
// Loads in pre-trained model, allows for retraining
// TODO: initially uses file path/name to refer to external data storage, but can be made switchable to asset objects later
// TODO: initially uses numbers for labels, later we want to support enums
//
UCLASS(/*TODO:Abstract*/)
class INTERACTMLSCRIPTING_API UInteractMLModelNode: public UK2Node
{
	GENERATED_BODY()

public:

private:

	//specialist
	UEdGraphPin* GetActorInputPin() const;
	UEdGraphPin* GetDataPathInputPin() const;
	UEdGraphPin* GetLiveParametersInputPin() const;
	UEdGraphPin* GetRunInputPin() const;
	UEdGraphPin* GetTrainingSetInputPin() const;
	UEdGraphPin* GetTrainInputPin() const;
	UEdGraphPin* GetResetInputPin() const;
	UEdGraphPin* GetLabelOutputPin() const;
	
protected:

	//~ Begin model specific overrides
	virtual UFunction* FindModelAccessFunction() const;// { check(false); return nullptr; }
	//~ End model specific overrides

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
	//virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	//virtual void PostReconstructNode();
	//~ End K2Node Interface};


	
private:

	UFunction* FindModelRunFunction() const;
	UFunction* FindModelTrainFunction() const;
	
};