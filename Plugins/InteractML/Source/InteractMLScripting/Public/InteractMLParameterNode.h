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
#include "InteractMLParameterNode.generated.h"


//general declarations

// we track and store the types of inputs attached to this node as parameter sources
// TODO: later we can enable renaming/editing of this list explicitly instead of just dragging onto wild nodes
//
USTRUCT()
struct FParameterSpec
{
	GENERATED_BODY()

	//pin identifier, used to resolve pins (encoded in name)
	UPROPERTY()
	int Identifier;

	//actual display name of pin (friendly name, so we can use name as ID)
	UPROPERTY()
	FString Name;
	
	//type needed to fully re-create the pin
	UPROPERTY()
	FEdGraphPinType Type;

	//setup
	FParameterSpec() : Identifier(0), Name(TEXT("")) {}
	FParameterSpec(FEdGraphPinType type, int id, FString name="") : Identifier(id), Name(name), Type(type) {}

	//re-gen display name and tooltip of a pin
	void ApplyPinType( UEdGraphPin* pin, FEdGraphPinType type ); //change/set
	void ApplyPinDisplayName( UEdGraphPin* pin, FText display_name ); //change/set
	void ApplyPinDisplayName( UEdGraphPin* pin ); //re-apply
	void ApplyPinTooltip(UEdGraphPin* pin) const; //generate, apply

	//access
	bool IsValid() { return Type.PinCategory != UEdGraphSchema_K2::PC_Wildcard; }
};


//main class

// Parameter collection custom blueprint node
// Allows any number of inputs, of various types, to be connected to be accumulated into a single UInteractMLParameters 
// collection object for passing around the graph and consumption by training and model nodes.
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLParameterNode : public UInteractMLNode, public IK2Node_AddPinInterface
{
	GENERATED_BODY()
	
	// parameters known to be expected as inputs to this node
	UPROPERTY()
	TArray<FParameterSpec> InputParameters;

public:

private:

	//specialist
	UEdGraphPin* GetParametersOutputPin() const;
	
protected:

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	//~ End UEdGraphNode Interface

	//~ Begin K2Node Interface
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual void PostReconstructNode();
	//~ End K2Node Interface

	// IK2Node_AddPinInterface interface
	virtual void AddInputPin() override;
	// End of IK2Node_AddPinInterface interface
	
private:
	int CountParameters() const; //only valid ones
	int CountFloats() const; //only valid ones

	//pin spec
	const FParameterSpec* FindPinSpec(const UEdGraphPin* pin) const;
	FParameterSpec* FindPinSpec(const UEdGraphPin* pin);
	
	//editing
	void RemoveParameterInput(UEdGraphPin* pin);

	//node expansion
	void CollectParameterPins( TMap<UEdGraphPin*,FParameterSpec*>& param_pins );
	UFunction* FindParameterAccessFunction() const;
	UFunction* FindParameterAddFunctionByType( FEdGraphPinType* pin_type );	
	
};