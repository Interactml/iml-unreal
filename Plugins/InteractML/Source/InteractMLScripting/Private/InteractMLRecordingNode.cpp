//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLRecordingNode.h"

//unreal
#include "BlueprintActionDatabaseRegistrar.h" //FBlueprintActionDatabaseRegistrar
#include "BlueprintNodeSpawner.h" //UBlueprintNodeSpawner
#include "EdGraphSchema_K2.h" //UEdGraphSchema_K2
#include "KismetCompiler.h" //FKismetCompilerContext
#include "K2Node_CallFunction.h" //UK2Node_Function
#include "Engine/SimpleConstructionScript.h" //USimpleConstructionScript
#include "BlueprintEditorUtils.h" //MarkBlueprintAsStructurallyModified
#include "ToolMenu.h" //UToolMenu
#include "ScopedTransaction.h" //FScopedTransaction
#include "K2Node_Self.h" //Self node

//module
#include "InteractMLTrainingSet.h"
#include "InteractMLBlueprintLibrary.h"
#include "InteractMLConstants.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES



// pin and function name constants
//

namespace FInteractMLRecordingNodePinNames
{
	//in
	static const FName TrainingSetInputPinName("Training Set");
	static const FName LiveParametersInputPinName("Live Parameters");
	static const FName LabelInputPinName("Label");
	static const FName RecordInputPinName("Record");
	static const FName ResetInputPinName( "Reset All" );
	//out
	static const FName ChangedOutputPinName("Changed");
}  	
namespace FInteractMLRecordingNodeFunctionNames
{
	static const FName RecordSingleFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RecordExample));
	static const FName RecordSeriesFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RecordExampleSeries));
}
namespace FInteractMLRecordingNodeRecordFunctionPinNames
{
	static const FName ActorPinName("Actor");
	static const FName TrainingSetPinName("TrainingSet");
	static const FName LiveParametersPinName("Parameters");
	static const FName LabelPinName("Label");
	static const FName RecordPinName("Record");
	static const FName ResetPinName("Reset");
	static const FName NodeIDPinName("NodeID");
}

/////////////////////////////////// HELPERS /////////////////////////////////////



//////////////////////////////// RECORDING NODE CLASS ////////////////////////////////////

// basic node properties
//
FText UInteractMLRecordingNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText node_name = LOCTEXT("RecordingNodeTitle", "Example Recorder");

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
		{
			FString title = node_name.ToString();
			switch (Mode)
			{
				case EInteractMLRecordingMode::Single:
					title.Append(TEXT("\n"));
					title.Append( LOCTEXT("RecordingNodeSubtitleSingle","Record single sample snaphot").ToString() );
					break;
				case EInteractMLRecordingMode::Series:
					title.Append(TEXT("\n"));
					title.Append( LOCTEXT("RecordingNodeSubtitleSeries","Record sample series").ToString() );
					break;
			}
			return FText::FromString(title);
		}

		case ENodeTitleType::MenuTitle:
		case ENodeTitleType::ListView:
		default:
		{
			FString title = node_name.ToString();
#if false //only one entry appears now since the node is configurable
			title.Append(TEXT(" ("));
			switch (Mode)
			{
				case EInteractMLRecordingMode::Single:
					title.Append( LOCTEXT("RecordingNodeModeSingle","Single").ToString() );
					break;
				case EInteractMLRecordingMode::Series:
					title.Append( LOCTEXT("RecordingNodeModeSeries","Series").ToString() );
					break;
			}
			title.Append(TEXT(")"));
#endif
			return FText::FromString(title);
		}
			
		case ENodeTitleType::EditableTitle:
			return FText(); //not editable
			break;
	}
}
FText UInteractMLRecordingNode::GetTooltipText() const
{
	switch (Mode)
	{
		case EInteractMLRecordingMode::Single:
			return LOCTEXT("RecordingNodeSubtitleSingle","Record a single parameter snapshot at a time for the current label");
		case EInteractMLRecordingMode::Series:
			return LOCTEXT("RecordingNodeSubtitleSeries","Record a series of parameter snapshots in a row for the current label");
		default:
			return FText();
	}
}


void UInteractMLRecordingNode::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	const FName PropertyName = e.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UInteractMLRecordingNode, Mode))
	{
		//rebuild because mode changed
		ReconstructNode();
	}
	
	Super::PostEditChangeProperty(e);
}

// custom pins
//
void UInteractMLRecordingNode::AllocateDefaultPins()
{
	//handle actor context pin
	Super::AllocateDefaultPins();

	//---- Inputs ----

	// Which trainging set to record into?	
	UEdGraphPin* trainingset_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteractMLTrainingSet::StaticClass(), FInteractMLRecordingNodePinNames::TrainingSetInputPinName );
	trainingset_pin->PinToolTip = LOCTEXT("RecordingNodeTrainingSetTooltip", "Training set asset to record to.").ToString();

	// parameters to record
	UEdGraphPin* liveparams_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParameters>::Get(), FInteractMLRecordingNodePinNames::LiveParametersInputPinName);
	liveparams_pin->PinToolTip = LOCTEXT("RecordingNodeLiveParamsPinTooltip", "The live parameters that will be recorded into the training set.").ToString();

	// label to associate with parameters being recorded
	UEdGraphPin* label_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, nullptr, FInteractMLRecordingNodePinNames::LabelInputPinName);
	label_pin->PinToolTip = LOCTEXT("RecordingNodeLabelPinTooltip", "The label to associate the current recording with.\nNOTE: Currently only numeric labels are supported.").ToString();

	// enable recording
	UEdGraphPin* record_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRecordingNodePinNames::RecordInputPinName);
	switch (Mode)
	{
		case EInteractMLRecordingMode::Single:
			record_pin->PinToolTip = LOCTEXT("RecordingNodeRecordPinTooltipSingle", "Set to true to record a single example snapshot for the current label.").ToString();
			break;
		case EInteractMLRecordingMode::Series:
			record_pin->PinToolTip = LOCTEXT("RecordingNodeRecordPinTooltipSeries", "Records a series of examples for the current label whilst true.").ToString();
			break;
	}

	// perform reset
	UEdGraphPin* reset_pin = CreatePin( EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRecordingNodePinNames::ResetInputPinName );
	reset_pin->PinToolTip = LOCTEXT( "RecordingNodeResetPinTooltip", "Set this to clear out all recorded training data and start again." ).ToString();

	//---- Outputs ----

	// change notification pin
	UEdGraphPin* changed_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRecordingNodePinNames::ChangedOutputPinName);
	changed_pin->PinToolTip = LOCTEXT("RecordingNodeChangedPinTooltip", "Becomes true briefly when new training data has been recorded.").ToString();
	
}


// pin access helpers : inputs
//
UEdGraphPin* UInteractMLRecordingNode::GetTrainingSetInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRecordingNodePinNames::TrainingSetInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLRecordingNode::GetLiveParametersInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRecordingNodePinNames::LiveParametersInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLRecordingNode::GetLabelInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRecordingNodePinNames::LabelInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLRecordingNode::GetRecordInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRecordingNodePinNames::RecordInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLRecordingNode::GetResetInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLRecordingNodePinNames::ResetInputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Input );
	return Pin;
}

// pin access helpers : outputs
//
UEdGraphPin* UInteractMLRecordingNode::GetChangedOutputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRecordingNodePinNames::ChangedOutputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}


// runtime node operation functionality hookup
//
void UInteractMLRecordingNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );

	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : data
	UEdGraphPin* MainTrainingSetPin = GetTrainingSetInputPin();
	UEdGraphPin* MainLiveParametersPin = GetLiveParametersInputPin();
	UEdGraphPin* MainLabelPin = GetLabelInputPin();
	UEdGraphPin* MainRecordPin = GetRecordInputPin();	
	UEdGraphPin* MainResetPin = GetResetInputPin();
	//output pins : exec (execution continues)
	UEdGraphPin* MainThenPin = FindPin( UEdGraphSchema_K2::PN_Then );	
	//output pins : data
	UEdGraphPin* MainChangedPin = GetChangedOutputPin();

	//internal example recording fn
	UFunction* RecordFn = FindRecordFunction();
	UK2Node_CallFunction* CallRecordFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallRecordFn->SetFromFunction( RecordFn );
	CallRecordFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallRecordFn, this);
	//record fn pins
	UEdGraphPin* RecordFnExecPin = CallRecordFn->GetExecPin();
	UEdGraphPin* RecordFnThenPin = CallRecordFn->GetThenPin();
	UEdGraphPin* RecordFnResultPin = CallRecordFn->GetReturnValuePin();
	UEdGraphPin* RecordFnActorPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::ActorPinName );
	UEdGraphPin* RecordFnTrainingSetPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::TrainingSetPinName );
	UEdGraphPin* RecordFnLiveParametersPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::LiveParametersPinName );
	UEdGraphPin* RecordFnLabelPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::LabelPinName );
	UEdGraphPin* RecordFnRecordPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::RecordPinName );
	UEdGraphPin* RecordFnResetPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::ResetPinName );
	UEdGraphPin* RecordFnNodeIDPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::NodeIDPinName );

	//chain functionality together
	CompilerContext.MovePinLinksToIntermediate( *MainExecPin, *RecordFnExecPin );
	CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *RecordFnThenPin );

	//generate node disambiguation/context
	FString NodeID = NodeGuid.ToString( EGuidFormats::Digits );

	//hook up recording fn pins
	ConnectContextActor(CompilerContext, SourceGraph, RecordFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainTrainingSetPin, *RecordFnTrainingSetPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLiveParametersPin, *RecordFnLiveParametersPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLabelPin, *RecordFnLabelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainRecordPin, *RecordFnRecordPin);
	CompilerContext.MovePinLinksToIntermediate(*MainResetPin, *RecordFnResetPin);
	RecordFnNodeIDPin->DefaultValue = NodeID;
	CompilerContext.MovePinLinksToIntermediate(*MainChangedPin, *RecordFnResultPin);

	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();
}

// locate function used to record more example data
//
UFunction* UInteractMLRecordingNode::FindRecordFunction() const
{
	FName record_function_name;
	switch (Mode)
	{
		case EInteractMLRecordingMode::Single:
			record_function_name = FInteractMLRecordingNodeFunctionNames::RecordSingleFunctionName;
			break;
		case EInteractMLRecordingMode::Series:
			record_function_name = FInteractMLRecordingNodeFunctionNames::RecordSeriesFunctionName;
			break;
		default:
			check(false);
			return nullptr;
	}
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( record_function_name );
}


#undef LOCTEXT_NAMESPACE
