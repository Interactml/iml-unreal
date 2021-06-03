//----
// InteractML - University Of Arts London
//----

#include "InteractMLTrainingSetNode.h"

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

namespace FInteractMLTrainingSetNodePinNames
{
	//in
	static const FName ActorInputPinName("Actor");
	static const FName DataPathInputPinName("Data Path");
	static const FName LiveParametersInputPinName("Live Parameters");
	static const FName LabelInputPinName("Label");
	static const FName RecordInputPinName("Record Example");
	//out
	static const FName TrainingSetOutputPinName("Training Set");
	static const FName ChangedOutputPinName("Changed");
}  	
namespace FInteractMLTrainingSetNodeFunctionNames
{
	static const FName AccessTrainingSetName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, GetTrainingSet));
	static const FName RecordExampleName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RecordExample));
}
namespace FTrainingSetNodeTrainingSetAccessPinNames
{
	static const FName ActorPinName("Actor");
	static const FName DataPathPinName("DataPath");
	static const FName NodeIDPinName("NodeID");
}
namespace FTrainingSetNodeRecordPinNames
{
	static const FName TrainingSetPinName("TrainingSet");
	static const FName LiveParametersPinName("Parameters");
	static const FName LabelPinName("Label");
	static const FName RecordPinName("Record");
	static const FName ModePinName("Mode");
	static const FName NodeIDPinName("NodeID");
}

/////////////////////////////////// HELPERS /////////////////////////////////////



//////////////////////////////// TRAINING SET NODE CLASS ////////////////////////////////////

// basic node properties
//
FText UInteractMLTrainingSetNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText node_name = LOCTEXT("TrainingSetNodeTitle", "Train The Machine");

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
		{
			FString title = node_name.ToString();
			title.Append(TEXT("\n"));
			title.Append( LOCTEXT("TrainingSetNodeSubTitle", "Single sample recording").ToString() );
			return FText::FromString(title);
		}

		case ENodeTitleType::MenuTitle:
		case ENodeTitleType::ListView:
		default:
			return node_name;

		case ENodeTitleType::EditableTitle:
			return FText(); //not editable
			break;
	}
}
FText UInteractMLTrainingSetNode::GetTooltipText() const
{
	return LOCTEXT("TrainingSetTooltip", "Load or record examples for training machine learning models");
}
FText UInteractMLTrainingSetNode::GetMenuCategory() const
{
	return LOCTEXT("TrainingSetNodeMenuCategory", "InteractML");
}
FLinearColor UInteractMLTrainingSetNode::GetNodeTitleColor() const
{
	return InteractMLConstants::NodeTitleColour.ReinterpretAsLinear(); 
}


// put node in blueprint menu
//
void UInteractMLTrainingSetNode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions( ActionRegistrar );
	
	UClass* Action = GetClass();
	if (ActionRegistrar.IsOpenForRegistration( Action ))
	{
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		check(Spawner != nullptr);
		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}

// custom pins
//
void UInteractMLTrainingSetNode::AllocateDefaultPins()
{
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	//---- Inputs ----

	// Target actor (needed for context)
	UEdGraphPin* actor_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, AActor::StaticClass(), FInteractMLTrainingSetNodePinNames::ActorInputPinName);
	actor_pin->PinToolTip = LOCTEXT("BlueprintNodeActorPinTooltip", "Interact ML nodes need an actor to provide context in which they operate.\nTypically this would be actor the graph is attached to (i.e. 'Self').").ToString();

	// Which data file to persist training data?	
	UEdGraphPin* data_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, nullptr, FInteractMLTrainingSetNodePinNames::DataPathInputPinName);
	data_pin->PinToolTip = LOCTEXT("TrainingSetNodeDataPinTooltip", "Path (optional) and Name to load/save training set data.").ToString();

	// parameters to record
	UEdGraphPin* liveparams_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParameters>::Get(), FInteractMLTrainingSetNodePinNames::LiveParametersInputPinName);
	liveparams_pin->PinToolTip = LOCTEXT("TrainingSetNodeLiveParamsPinTooltip", "The live parameters that will be recorded into the training set.").ToString();

	// label to associate with parameters being recorded
	UEdGraphPin* label_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, nullptr, FInteractMLTrainingSetNodePinNames::LabelInputPinName);
	label_pin->PinToolTip = LOCTEXT("TrainingSetNodeLabelPinTooltip", "The label to associate the current recording with.\nNOTE: Currently only numeric labels are supported.").ToString();

	// enable recording
	UEdGraphPin* record_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLTrainingSetNodePinNames::RecordInputPinName);
	record_pin->PinToolTip = LOCTEXT("TrainingSetNodeRecordPinTooltip", "Set this to record an example for the current label.").ToString();
	
	//---- Outputs ----

	// Resulting training set (ptr) struct
	UEdGraphPin* trainingset_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, UInteractMLTrainingSet::StaticClass(), FInteractMLTrainingSetNodePinNames::TrainingSetOutputPinName);
	trainingset_pin->PinToolTip = LOCTEXT("TrainingSetNodeOutputPinTooltip", "Set of examples for training machine learning models.").ToString();

	// change notification pin
	UEdGraphPin* changed_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLTrainingSetNodePinNames::ChangedOutputPinName);
	changed_pin->PinToolTip = LOCTEXT("TrainingSetNodeChangedPinTooltip", "Becomes true briefly when new training data has been recorded.").ToString();
	
	Super::AllocateDefaultPins();
}


// pin access helpers : inputs
//
UEdGraphPin* UInteractMLTrainingSetNode::GetActorInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingSetNodePinNames::ActorInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLTrainingSetNode::GetDataPathInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingSetNodePinNames::DataPathInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLTrainingSetNode::GetLiveParametersInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingSetNodePinNames::LiveParametersInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLTrainingSetNode::GetLabelInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingSetNodePinNames::LabelInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLTrainingSetNode::GetRecordInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingSetNodePinNames::RecordInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

// pin access helpers : outputs
//
UEdGraphPin* UInteractMLTrainingSetNode::GetTrainingSetOutputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingSetNodePinNames::TrainingSetOutputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}
UEdGraphPin* UInteractMLTrainingSetNode::GetChangedOutputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingSetNodePinNames::ChangedOutputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}


// runtime node operation functionality hookup
//
void UInteractMLTrainingSetNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );

	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : data
	UEdGraphPin* MainActorPin = GetActorInputPin();
	UEdGraphPin* MainDataPathPin = GetDataPathInputPin();
	UEdGraphPin* MainLiveParametersPin = GetLiveParametersInputPin();
	UEdGraphPin* MainLabelPin = GetLabelInputPin();
	UEdGraphPin* MainRecordPin = GetRecordInputPin();	
	//output pins : exec (execution continues)
	UEdGraphPin* MainThenPin = FindPin( UEdGraphSchema_K2::PN_Then );	
	//output pins : data
	UEdGraphPin* MainOutputPin = GetTrainingSetOutputPin();
	UEdGraphPin* MainChangedPin = GetChangedOutputPin();

	//internal training set accessor
	UFunction* TrainingSetFn = FindTrainingSetAccessFunction();
	UK2Node_CallFunction* CallTrainingSetFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallTrainingSetFn->SetFromFunction( TrainingSetFn );
	CallTrainingSetFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallTrainingSetFn, this);
	//training set fn pins
	UEdGraphPin* TrainingSetFnExecPin = CallTrainingSetFn->GetExecPin();
	UEdGraphPin* TrainingSetFnThenPin = CallTrainingSetFn->GetThenPin();
	UEdGraphPin* TrainingSetFnResultPin = CallTrainingSetFn->GetReturnValuePin();
	UEdGraphPin* TrainingSetFnActorPin = CallTrainingSetFn->FindPinChecked( FTrainingSetNodeTrainingSetAccessPinNames::ActorPinName );
	UEdGraphPin* TrainingSetFnDataPathPin = CallTrainingSetFn->FindPinChecked( FTrainingSetNodeTrainingSetAccessPinNames::DataPathPinName );
	UEdGraphPin* TrainingSetFnNodeIDPin = CallTrainingSetFn->FindPinChecked( FTrainingSetNodeTrainingSetAccessPinNames::NodeIDPinName );
	
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
	UEdGraphPin* RecordFnTrainingSetPin = CallRecordFn->FindPinChecked( FTrainingSetNodeRecordPinNames::TrainingSetPinName );
	UEdGraphPin* RecordFnLiveParametersPin = CallRecordFn->FindPinChecked( FTrainingSetNodeRecordPinNames::LiveParametersPinName );
	UEdGraphPin* RecordFnLabelPin = CallRecordFn->FindPinChecked( FTrainingSetNodeRecordPinNames::LabelPinName );
	UEdGraphPin* RecordFnRecordPin = CallRecordFn->FindPinChecked( FTrainingSetNodeRecordPinNames::RecordPinName );
	UEdGraphPin* RecordFnModePin = CallRecordFn->FindPinChecked( FTrainingSetNodeRecordPinNames::ModePinName );
	UEdGraphPin* RecordFnNodeIDPin = CallRecordFn->FindPinChecked( FTrainingSetNodeRecordPinNames::NodeIDPinName );

	//chain execution pins (training access, then recording)
	CompilerContext.MovePinLinksToIntermediate( *MainExecPin, *TrainingSetFnExecPin );
	TrainingSetFnThenPin->MakeLinkTo( RecordFnExecPin );
	CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *RecordFnThenPin );

	//generate node disambiguation/context
	FString NodeID = NodeGuid.ToString( EGuidFormats::Digits );

	//hook up training set access pins
	CompilerContext.MovePinLinksToIntermediate(*MainActorPin, *TrainingSetFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainDataPathPin, *TrainingSetFnDataPathPin);
	TrainingSetFnNodeIDPin->DefaultValue = NodeID;
	CompilerContext.MovePinLinksToIntermediate(*MainOutputPin, *TrainingSetFnResultPin);
	
	//hook up recording fn pins
	TrainingSetFnResultPin->MakeLinkTo(RecordFnTrainingSetPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLiveParametersPin, *RecordFnLiveParametersPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLabelPin, *RecordFnLabelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainRecordPin, *RecordFnRecordPin);
	RecordFnModePin->DefaultValue = TEXT("0");	//default (single)
	RecordFnNodeIDPin->DefaultValue = NodeID;
	CompilerContext.MovePinLinksToIntermediate(*MainChangedPin, *RecordFnResultPin);

	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();
}

// locate function used to obtain the training set object
//
UFunction* UInteractMLTrainingSetNode::FindTrainingSetAccessFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( FInteractMLTrainingSetNodeFunctionNames::AccessTrainingSetName );
}

// locate function used to record more example data
//
UFunction* UInteractMLTrainingSetNode::FindRecordFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( FInteractMLTrainingSetNodeFunctionNames::RecordExampleName );
}


#undef LOCTEXT_NAMESPACE
