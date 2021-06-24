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
#include "InteractMLTrainingSetNode.generated.h"


//general declarations

// Training set load/save and recording blueprint base node
// Loads in pre-recorded training examples, allows for recording more examples, and provides training data for models
// Derived implementations specialise for single/series operation
// TODO: initially uses file path/name to refer to external data storage, but can be made switchable to asset objects later
// TODO: initially uses numbers for labels, later we want to support enums
//
UCLASS(Abstract)
class INTERACTMLSCRIPTING_API UInteractMLTrainingSetNode: public UK2Node
{
	GENERATED_BODY()


public:

private:

	//specialist
	UEdGraphPin* GetActorInputPin() const;
	UEdGraphPin* GetDataPathInputPin() const;
	UEdGraphPin* GetLiveParametersInputPin() const;
	UEdGraphPin* GetLabelInputPin() const;
	UEdGraphPin* GetRecordInputPin() const;
	UEdGraphPin* GetResetInputPin() const;
	UEdGraphPin* GetTrainingSetOutputPin() const;
	UEdGraphPin* GetChangedOutputPin() const;
	
protected:

	//~ Begin training-set specific overrides
	virtual FText GetTrainingSetName() const { check(false); return FText(); }
	virtual FText GetTrainingSetDescription() const { check(false); return FText(); }
	virtual FText GetTrainingSetTooltip() const { check(false); return FText(); }
	virtual FText GetRecordPinTooltip() const { check(false); return FText(); } 
	virtual FName GetRecordFunctionName() const { check(false); return NAME_None; }
	//~ End training-set specific overrides
	
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

	UFunction* FindTrainingSetAccessFunction() const;
	UFunction* FindRecordFunction() const;
	
};