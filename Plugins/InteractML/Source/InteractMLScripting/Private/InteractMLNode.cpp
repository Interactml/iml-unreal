//----
// InteractML - University Of Arts London
//----

#include "InteractMLNode.h"

//unreal
#include "BlueprintActionDatabaseRegistrar.h" //FBlueprintActionDatabaseRegistrar
#include "BlueprintNodeSpawner.h" //UBlueprintNodeSpawner
#include "EdGraphSchema_K2.h" //UEdGraphSchema_K2
#include "KismetCompiler.h" //FKismetCompilerContext
#include "K2Node_CallFunction.h" //UK2Node_Function
#include "Engine/SimpleConstructionScript.h" //USimpleConstructionScript
#include "Kismet2/BlueprintEditorUtils.h" //MarkBlueprintAsStructurallyModified
#include "ToolMenu.h" //UToolMenu
#include "ScopedTransaction.h" //FScopedTransaction
#include "K2Node_Self.h" //Self

//module
#include "InteractMLModel.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLBlueprintLibrary.h"
#include "InteractMLConstants.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES



/////////////////////////////////// HELPERS /////////////////////////////////////



//////////////////////////////// TRAINING NODE CLASS ////////////////////////////////////

// basic node properties
//
FText UInteractMLNode::GetMenuCategory() const
{
	return InteractMLConstants::NodeMenuCategory;
}
FLinearColor UInteractMLNode::GetNodeTitleColor() const
{
	return InteractMLConstants::NodeTitleColour.ReinterpretAsLinear(); 
}


// put node in blueprint menu
//
void UInteractMLNode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions( ActionRegistrar );
	
	UClass* Action = GetClass();
	if (!Action->HasAnyClassFlags(CLASS_Abstract))
	{
		if (ActionRegistrar.IsOpenForRegistration(Action))
		{
			UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
			check(Spawner != nullptr);
			ActionRegistrar.AddBlueprintAction(Action, Spawner);
		}
	}
}


void UInteractMLNode::PostEditChangeProperty( struct FPropertyChangedEvent& e )
{
	const FName PropertyName = e.GetPropertyName();
	if(PropertyName == GET_MEMBER_NAME_CHECKED( UInteractMLNode, bCustomContextActor ))
	{
		//context actor option changed, rebuild
		ReconstructNode();
	}

	Super::PostEditChangeProperty( e );
}


// custom pins, call first in derived implementation
//
void UInteractMLNode::AllocateDefaultPins()
{
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	//---- Inputs ----

	// Optional target actor (needed for context)
	if(bCustomContextActor)
	{
		UEdGraphPin* actor_pin = CreatePin( EGPD_Input, UEdGraphSchema_K2::PC_Object, AActor::StaticClass(), InteractMLConstants::BlueprintNodeActorPinName );
		actor_pin->PinToolTip = InteractMLConstants::BlueprintNodeActorPinTooltip.ToString();
	}
	
	Super::AllocateDefaultPins(); //benign
}


// pin access helpers : inputs
//
UEdGraphPin* UInteractMLNode::GetActorInputPin() const
{
	if(bCustomContextActor)
	{
		UEdGraphPin* Pin = FindPin( InteractMLConstants::BlueprintNodeActorPinName );
		check( Pin == NULL || Pin->Direction == EGPD_Input );
		return Pin;
	}
	return nullptr;
}




// runtime node operation functionality hookup
//
void UInteractMLNode::ConnectContextActor(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph, UEdGraphPin* ActorRecipientPin )
{
	//hook up train fn pins
	UEdGraphPin* MainActorPin = GetActorInputPin();
	if(MainActorPin) //optional
	{
		CompilerContext.MovePinLinksToIntermediate( *MainActorPin, *ActorRecipientPin );
	}
	else
	{
		//context actor fallback is self
		UK2Node_Self* SelfNode = SourceGraph->CreateIntermediateNode<UK2Node_Self>();
		SelfNode->AllocateDefaultPins();
		UEdGraphPin* InternalActorPin = SelfNode->FindPinChecked( UEdGraphSchema_K2::PN_Self );
		InternalActorPin->MakeLinkTo( ActorRecipientPin );
	}
}


#undef LOCTEXT_NAMESPACE
