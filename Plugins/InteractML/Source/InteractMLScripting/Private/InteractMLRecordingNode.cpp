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
#include "Kismet2/BlueprintEditorUtils.h" //MarkBlueprintAsStructurallyModified
#include "ToolMenu.h" //UToolMenu
#include "ScopedTransaction.h" //FScopedTransaction
#include "K2Node_Self.h" //Self node

//module
#include "InteractMLTrainingSet.h"
#include "InteractMLBlueprintLibrary.h"
#include "InteractMLConstants.h"
#include "InteractMLLabel.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES


// pin and function name constants
//
namespace FInteractMLRecordingNodePinNames
{
	//NOTE: these are localised in AllocateDefaultPins, don't change name here or you break existing graphs
	//in
	static const FName TrainingSetInputPinName("TrainingSet");		
	static const FName LiveParametersInputPinName("Parameters");
	static const FName LabelInputPinName("Label");
	static const FName RecordInputPinName("Record");
	static const FName DeleteAllInputPinName( "DeleteAll" );
	static const FName DeleteLabelInputPinName( "DeleteLabel" );
	static const FName DeleteLastInputPinName( "DeleteLast" );
	//out
	static const FName ChangedOutputPinName("Changed");
}  	
namespace FInteractMLRecordingNodeFunctionNames
{
	static const FName RecordSimpleLabelFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RecordExampleSimple));
	static const FName RecordCompositeLabelFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RecordExampleComposite));
}
//UInteractMLBlueprintLibrary::RecordExampleSimple/Composite(...)
namespace FInteractMLRecordingNodeRecordFunctionPinNames
{
	static const FName ActorPinName("Actor");
	static const FName TrainingSetPinName("TrainingSet");
	static const FName LiveParametersPinName("Parameters");
	static const FName WantSeriesPinName("WantSeries");
	static const FName RecordPinName("Record");
	static const FName DeleteAllPinName("DeleteAll");
	static const FName DeleteLabelPinName("DeleteLabel");
	static const FName DeleteLastPinName("DeleteLast");
	static const FName NodeIDPinName("NodeID");
	//simple label
	static const FName LabelPinName("Label");
	//composite label
	static const FName LabelTypePinName("LabelType");
	static const FName LabelDataPinName("LabelData");
}

/////////////////////////////////// HELPERS /////////////////////////////////////


//////////////////////////////// RECORDING NODE CLASS ////////////////////////////////////

// node title
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
			return FText::FromString(title);
		}
			
		case ENodeTitleType::EditableTitle:
			return FText(); //not editable
			break;
	}
}

// node tooltip
//
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

// monitor property changes that may invalidate node structure (e.g. exposed pins or title)
//
void UInteractMLRecordingNode::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	const FName PropertyName = e.GetPropertyName();

	//mode and label both affect pins
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UInteractMLRecordingNode, Mode)
	  ||PropertyName == GET_MEMBER_NAME_CHECKED(UInteractMLRecordingNode, LabelType))
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
	trainingset_pin->PinFriendlyName = LOCTEXT("RecordingNodeTrainingSetInputPinName", "Training Set");

	// parameters to record
	UEdGraphPin* liveparams_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParameters>::Get(), FInteractMLRecordingNodePinNames::LiveParametersInputPinName);
	liveparams_pin->PinToolTip = LOCTEXT("RecordingNodeLiveParamsPinTooltip", "The live parameters that will be recorded into the training set.").ToString();
	liveparams_pin->PinFriendlyName = LOCTEXT("RecordingNodeParameterInputPinName", "Live Parameters");

	// label to associate with parameters being recorded
	if (LabelType)
	{
		//composite label
		UEdGraphPin* label_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, LabelType, FInteractMLRecordingNodePinNames::LabelInputPinName);
		label_pin->PinToolTip = LOCTEXT("RecordingNodeCompositeLabelPinTooltip", "The set of label values expected as output to be associated with the current recorded parameters.").ToString();
		label_pin->PinFriendlyName = LOCTEXT("RecordingNodeLabelInputPinName", "Expected Output");
	}
	else
	{
		//simple numerical label
		UEdGraphPin* label_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, nullptr, FInteractMLRecordingNodePinNames::LabelInputPinName);
		label_pin->PinToolTip = LOCTEXT("RecordingNodeNumericLabelPinTooltip", "The numeric label expected as output to be associated with the current recorded parameters.").ToString();
		label_pin->PinFriendlyName = LOCTEXT("RecordingNodeLabelInputPinName", "Expected Output");
	}

	// enable recording
	UEdGraphPin* record_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRecordingNodePinNames::RecordInputPinName);
	record_pin->PinFriendlyName = LOCTEXT("RecordingNodeRecordInputPinName", "Record");
	switch (Mode)
	{
		case EInteractMLRecordingMode::Single:
			record_pin->PinToolTip = LOCTEXT("RecordingNodeRecordPinTooltipSingle", "Set to true to record a single example snapshot for the current label.").ToString();
			break;
		case EInteractMLRecordingMode::Series:
			record_pin->PinToolTip = LOCTEXT("RecordingNodeRecordPinTooltipSeries", "Records a series of examples for the current label whilst true.").ToString();
			break;
	}

	// various delete/reset operations
	UEdGraphPin* deletelast_pin = CreatePin( EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRecordingNodePinNames::DeleteLastInputPinName );
	deletelast_pin->PinToolTip = LOCTEXT( "RecordingNodeDeleteLastPinTooltip", "Set this to clear just the most recently recorded example." ).ToString();
	deletelast_pin->PinFriendlyName = LOCTEXT("RecordingNodeDeleteLastInputPinName", "Delete Last");
	UEdGraphPin* deletelabel_pin = CreatePin( EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRecordingNodePinNames::DeleteLabelInputPinName );
	deletelabel_pin->PinToolTip = LOCTEXT( "RecordingNodeDeleteLabelPinTooltip", "Set this to clear out the examples recorded for the current Expected Output." ).ToString();
	deletelabel_pin->PinFriendlyName = LOCTEXT("RecordingNodeDeleteLabelInputPinName", "Delete Output");
	UEdGraphPin* deleteall_pin = CreatePin( EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRecordingNodePinNames::DeleteAllInputPinName );
	deleteall_pin->PinToolTip = LOCTEXT( "RecordingNodeDeleteAllPinTooltip", "Set this to clear out ALL recorded training data and start again." ).ToString();
	deleteall_pin->PinFriendlyName = LOCTEXT("RecordingNodeDeleteAllInputPinName", "Delete All");
	
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
UEdGraphPin* UInteractMLRecordingNode::GetDeleteLastInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLRecordingNodePinNames::DeleteLastInputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Input );
	return Pin;
}
UEdGraphPin* UInteractMLRecordingNode::GetDeleteLabelInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLRecordingNodePinNames::DeleteLabelInputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Input );
	return Pin;
}
UEdGraphPin* UInteractMLRecordingNode::GetDeleteAllInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLRecordingNodePinNames::DeleteAllInputPinName );
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
	UEdGraphPin* MainDeleteLastPin = GetDeleteLastInputPin();
	UEdGraphPin* MainDeleteLabelPin = GetDeleteLabelInputPin();
	UEdGraphPin* MainDeleteAllPin = GetDeleteAllInputPin();
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
	UEdGraphPin* RecordFnWantSeriesPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::WantSeriesPinName );
	UEdGraphPin* RecordFnLiveParametersPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::LiveParametersPinName );
	UEdGraphPin* RecordFnRecordPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::RecordPinName );
	UEdGraphPin* RecordFnDeleteLastPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::DeleteLastPinName );
	UEdGraphPin* RecordFnDeleteLabelPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::DeleteLabelPinName );
	UEdGraphPin* RecordFnDeleteAllPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::DeleteAllPinName );
	UEdGraphPin* RecordFnNodeIDPin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::NodeIDPinName );	
	//label pins
	UEdGraphPin* RecordFnLabelPin = nullptr; 
	if(LabelType!=nullptr)
	{
		//composite label operation (passed struct)
		RecordFnLabelPin = CallRecordFn->FindPinChecked(FInteractMLRecordingNodeRecordFunctionPinNames::LabelDataPinName);
		//force generic struct to be the known struct type for passing
		RecordFnLabelPin->PinType = MainLabelPin->PinType;
		//there is a type pin to set up too
		UEdGraphPin* RecordFnLabelTypePin = CallRecordFn->FindPinChecked( FInteractMLRecordingNodeRecordFunctionPinNames::LabelTypePinName );
		RecordFnLabelTypePin->DefaultObject = LabelType;
	}
	else
	{
		//simple label operation (direct float value)
		RecordFnLabelPin = CallRecordFn->FindPinChecked(FInteractMLRecordingNodeRecordFunctionPinNames::LabelPinName);
	}
	
	//chain functionality together
	CompilerContext.MovePinLinksToIntermediate( *MainExecPin, *RecordFnExecPin );
	CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *RecordFnThenPin );

	//generate node disambiguation/context
	FString NodeID = NodeGuid.ToString( EGuidFormats::Digits );

	//hook up rest of recording fn pins
	ConnectContextActor(CompilerContext, SourceGraph, RecordFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainTrainingSetPin, *RecordFnTrainingSetPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLiveParametersPin, *RecordFnLiveParametersPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLabelPin, *RecordFnLabelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainRecordPin, *RecordFnRecordPin);
	CompilerContext.MovePinLinksToIntermediate(*MainDeleteLastPin, *RecordFnDeleteLastPin);
	CompilerContext.MovePinLinksToIntermediate(*MainDeleteLabelPin, *RecordFnDeleteLabelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainDeleteAllPin, *RecordFnDeleteAllPin);
	RecordFnNodeIDPin->DefaultValue = NodeID;
	RecordFnWantSeriesPin->DefaultValue = (Mode == EInteractMLRecordingMode::Series)?TEXT("true"):TEXT("false");
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
	if (LabelType)
	{
		//composite label	
		record_function_name = FInteractMLRecordingNodeFunctionNames::RecordCompositeLabelFunctionName;
	}
	else
	{
		//simple label
		record_function_name = FInteractMLRecordingNodeFunctionNames::RecordSimpleLabelFunctionName;
	}
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( record_function_name );
}


#undef LOCTEXT_NAMESPACE
