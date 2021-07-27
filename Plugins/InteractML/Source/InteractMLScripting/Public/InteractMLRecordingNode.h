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
// TODO: initially uses numbers for labels, later we want to support enums
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLRecordingNode: public UInteractMLNode
{
	GENERATED_BODY()

public:
	//single or series sample recording?
	UPROPERTY(EditAnywhere,meta=(Tooltip="Record a single value each time activated or a continuous series whilst active?"))
	EInteractMLRecordingMode Mode;

	//label type (expected outputs)
	UPROPERTY(EditAnywhere,meta=(HideViewOptions,ShowDisplayNames,Tooltip="Specify composite label type by selecting a Label Type asset that defines the output parameters to train against, leave None for simple numerical output"))
	UInteractMLLabel* LabelType;

private:

	//specialist
	UEdGraphPin* GetTrainingSetInputPin() const;
	UEdGraphPin* GetLiveParametersInputPin() const;
	UEdGraphPin* GetLabelInputPin() const;
	UEdGraphPin* GetRecordInputPin() const;
	UEdGraphPin* GetResetInputPin() const;
	UEdGraphPin* GetChangedOutputPin() const;
	
protected:
	
	//~ Begin UObject Interface
	//virtual void Serialize(FArchive& Ar) override;	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
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
	virtual bool ShouldShowNodeProperties() const override { return true; } //We have configuration properties we need the user to have access to
	//virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	//virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	//virtual void PostReconstructNode();
	//~ End K2Node Interface};


	
private:

	UFunction* FindRecordFunction() const;
	
};