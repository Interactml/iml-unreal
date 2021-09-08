//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLExternalTrainingSetNode.h"

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

//module
#include "InteractMLModel.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLBlueprintLibrary.h"
#include "InteractMLConstants.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES


// pin and function name constants
//
namespace FInteractMLExternalTrainingSetNodePinNames
{
	//in
	static const FName DataPathInputPinName("Data Path");
	//out
	static const FName TrainingSetOutputPinName("Training Set");
	static const FName HasDataOutputPinName("Has Data?");
}  	
namespace FInteractMLExternalTrainingSetNodeFunctionNames
{
	static const FName GetTrainingSetFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, GetTrainingSet));
}
//UInteractMLBlueprintLibrary::GetTrainingSet(...)
namespace FInteractMLExternalTrainingSetNodeTrainingSetAccessFunctionPinNames
{
	static const FName AcotrPinName("Actor");
	static const FName DataPathPinName("DataPath");
	static const FName NodeIDPinName("NodeID");
	static const FName HasDataPinName("HasData");
}

/////////////////////////////////// HELPERS /////////////////////////////////////


////////////////////// EXTERNAL TRAINING SET NODE CLASS /////////////////////////

// node title
//
FText UInteractMLExternalTrainingSetNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString title = LOCTEXT("ExternalTrainingSetNodeTitle", "Training Set").ToString();

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
			title.Append(TEXT("\n"));
			title.Append( LOCTEXT("ExternalTrainingSetNodeSubTitle", "External training set data file").ToString() );
			break;

		case ENodeTitleType::MenuTitle:
		case ENodeTitleType::ListView:
		default:
			title.Append(TEXT(" ("));
			title.Append(LOCTEXT("ExternalTrainingSetNodeMenuDesc", "External").ToString());
			title.Append(TEXT(")"));
			break;

		case ENodeTitleType::EditableTitle:
			title = ""; //not editable
			break;
	}

	return FText::FromString(title);
}

// node tooltip
//
FText UInteractMLExternalTrainingSetNode::GetTooltipText() const
{
	return LOCTEXT("ExternalTrainingSetNodeTooltip", "Directly access an external training set data file");
}

// custom pins
//
void UInteractMLExternalTrainingSetNode::AllocateDefaultPins()
{
	//handle context actor pin
	Super::AllocateDefaultPins();

	//---- Inputs ----
	
	// Which data file to persist training data?	
	UEdGraphPin* data_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, nullptr, FInteractMLExternalTrainingSetNodePinNames::DataPathInputPinName);
	data_pin->PinToolTip = LOCTEXT("ExternalTrainingSetNodeDataPathPinTooltip", "Path (optional) and Name to load/save training set data.").ToString();
	
	//---- Outputs ----
	
	// Resulting training set object
	UEdGraphPin* trainingset_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, UInteractMLTrainingSet::StaticClass(), FInteractMLExternalTrainingSetNodePinNames::TrainingSetOutputPinName);
	trainingset_pin->PinToolTip = LOCTEXT("ExternalTrainingSetNodeOutputPinTooltip", "Set of examples for training machine learning models.").ToString();
	
	//has data? pin
	UEdGraphPin* has_data_pin = CreatePin( EGPD_Output, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLExternalTrainingSetNodePinNames::HasDataOutputPinName );
	has_data_pin->PinToolTip = LOCTEXT( "ExternalTrainingSetNodeHasDataPinTooltip", "Indicated whether the current training set has any recorded examples." ).ToString();
}

// pin access helpers : inputs
//
UEdGraphPin* UInteractMLExternalTrainingSetNode::GetDataPathInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLExternalTrainingSetNodePinNames::DataPathInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

// pin access helpers : outputs
//
UEdGraphPin* UInteractMLExternalTrainingSetNode::GetTrainingSetOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLExternalTrainingSetNodePinNames::TrainingSetOutputPinName);
	check( Pin == NULL || Pin->Direction == EGPD_Output );
	return Pin;
}
UEdGraphPin* UInteractMLExternalTrainingSetNode::GetHasDataOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLExternalTrainingSetNodePinNames::HasDataOutputPinName);
	check( Pin == NULL || Pin->Direction == EGPD_Output );
	return Pin;
}

// runtime node operation functionality hookup
//
void UInteractMLExternalTrainingSetNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );
	
	//generate node disambiguation/context
	FString NodeID = NodeGuid.ToString( EGuidFormats::Digits );
	
	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : data
	UEdGraphPin* MainDataPathPin = GetDataPathInputPin();
	//output pins : exec (execution continues)
	UEdGraphPin* MainThenPin = FindPin( UEdGraphSchema_K2::PN_Then );	
	//output pins : data
	UEdGraphPin* MainTrainingSetOutputPin = GetTrainingSetOutputPin();
	UEdGraphPin* MainHasDataOutputPin = GetHasDataOutputPin();
	
	//internal model training fn
	UFunction* AccessFn = FindTrainingSetFunction();
	UK2Node_CallFunction* CallAccessFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallAccessFn->SetFromFunction( AccessFn );
	CallAccessFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallAccessFn, this);
	//training set access fn pins
	UEdGraphPin* AccessFnExecPin = CallAccessFn->GetExecPin();
	UEdGraphPin* AccessFnThenPin = CallAccessFn->GetThenPin();
	UEdGraphPin* AccessFnResultPin = CallAccessFn->GetReturnValuePin();
	UEdGraphPin* AccessFnActorPin = CallAccessFn->FindPinChecked( FInteractMLExternalTrainingSetNodeTrainingSetAccessFunctionPinNames::AcotrPinName );
	UEdGraphPin* AccessFnDataPathPin = CallAccessFn->FindPinChecked( FInteractMLExternalTrainingSetNodeTrainingSetAccessFunctionPinNames::DataPathPinName );
	UEdGraphPin* AccessFnNodeIDPin = CallAccessFn->FindPinChecked( FInteractMLExternalTrainingSetNodeTrainingSetAccessFunctionPinNames::NodeIDPinName );
	UEdGraphPin* AccessFnHasDataPin = CallAccessFn->FindPinChecked( FInteractMLExternalTrainingSetNodeTrainingSetAccessFunctionPinNames::HasDataPinName );
	
	//chain functionality together
	CompilerContext.MovePinLinksToIntermediate(*MainExecPin, *AccessFnExecPin);
	CompilerContext.MovePinLinksToIntermediate(*MainThenPin, *AccessFnThenPin);
	
	//hook up train fn pins
	ConnectContextActor(CompilerContext, SourceGraph, AccessFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainDataPathPin, *AccessFnDataPathPin);
	AccessFnNodeIDPin->DefaultValue = NodeID;
	CompilerContext.MovePinLinksToIntermediate(*MainTrainingSetOutputPin, *AccessFnResultPin);
	CompilerContext.MovePinLinksToIntermediate(*MainHasDataOutputPin, *AccessFnHasDataPin);
	
	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();
}

// locate function used to train the model
//
UFunction* UInteractMLExternalTrainingSetNode::FindTrainingSetFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( FInteractMLExternalTrainingSetNodeFunctionNames::GetTrainingSetFunctionName );
}

#undef LOCTEXT_NAMESPACE
