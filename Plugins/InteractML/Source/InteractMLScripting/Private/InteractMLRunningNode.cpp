//----
// InteractML - University Of Arts London
//----

#include "InteractMLRunningNode.h"

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



// pin and function name constants
//

namespace FInteractMLRunningNodePinNames
{
	//in
	static const FName ModelInputPinName("Model");
	static const FName LiveParametersInputPinName("Live Parameters");
	static const FName RunInputPinName("Run");
	//out
	static const FName LabelOutputPinName("Label");
}  	
namespace FInteractMLRunningNodeFunctionNames
{
	static const FName RunModelSimpleFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RunModelSimple));
	static const FName RunModelCompositeFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RunModelComposite));
}
//UInteractMLBlueprintLibrary::RunModel(...)
namespace FInteractMLRunningNodeRunModelPinNames
{
	static const FName ActorPinName( "Actor" );
	static const FName ModelPinName("Model");
	static const FName LiveParametersPinName("Parameters");
	static const FName RunPinName("Run");
	static const FName NodeIDPinName("NodeID");
	//composite
	static const FName LabelTypePinName("LabelType");
	static const FName LabelDataPinName("LabelData");
}

/////////////////////////////////// HELPERS /////////////////////////////////////



//////////////////////////////// MODEL NODE (BASE) CLASS ////////////////////////////////////

// basic node properties
//
FText UInteractMLRunningNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString title = LOCTEXT("RunningNodeTitle", "Machine Learning Robot").ToString();

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
//			title.Append(TEXT("\n"));
//			title.Append( LOCTEXT("ModelNodeSubTitle", "Machine Learning System").ToString() );
			break;

		case ENodeTitleType::MenuTitle:
		case ENodeTitleType::ListView:
		default:
			break;

		case ENodeTitleType::EditableTitle:
			title = ""; //not editable
			break;
	}

	return FText::FromString(title);
}
FText UInteractMLRunningNode::GetTooltipText() const
{
	return LOCTEXT("RunningNodeTooltip", "Runs a machine learning model on live parameters to find a label");
}

void UInteractMLRunningNode::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	const FName PropertyName = e.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UInteractMLRunningNode, LabelType))
	{
		//rebuild because mode changed
		ReconstructNode();
	}
	
	Super::PostEditChangeProperty(e);
}

// custom pins
//
void UInteractMLRunningNode::AllocateDefaultPins()
{
	//handle actor context pin
	Super::AllocateDefaultPins();

	//---- Inputs ----

	// Which model to run?	
	UEdGraphPin* model_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteractMLModel::StaticClass(), FInteractMLRunningNodePinNames::ModelInputPinName);
	model_pin->PinToolTip = LOCTEXT("RunningNodeModelPinTooltip", "The model to run.").ToString();

	// parameters to match
	UEdGraphPin* liveparams_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParameters>::Get(), FInteractMLRunningNodePinNames::LiveParametersInputPinName);
	liveparams_pin->PinToolTip = LOCTEXT("RunningNodeLiveParamsPinTooltip", "The live parameters that the model will be running against.").ToString();

	// enable run
	UEdGraphPin* run_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRunningNodePinNames::RunInputPinName);
	run_pin->PinToolTip = LOCTEXT("RunningNodeRunPinTooltip", "Set this to run the model against the current set of input parameters.\nFor series matching models (e.g. DTW) enable to record inputs disable to run the model on the accumulated samples.").ToString();

	//---- Outputs ----

	// label to produce with parameters being matched
	if (LabelType)
	{
		//composite label
		UEdGraphPin* label_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, LabelType, FInteractMLRunningNodePinNames::LabelOutputPinName);
		label_pin->PinToolTip = LOCTEXT("RunningNodeCompositeLabelPinTooltip", "Result of running the model on the input parameters.").ToString();
	}
	else
	{
		//simple numerical label
		UEdGraphPin* label_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Float, nullptr, FInteractMLRunningNodePinNames::LabelOutputPinName);
		label_pin->PinToolTip = LOCTEXT("RecordingNodeSimpleNumericLabelPinTooltip", "Result of running the model on the input parameters.").ToString();
	}
	
}


// pin access helpers : inputs
//
UEdGraphPin* UInteractMLRunningNode::GetModelInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRunningNodePinNames::ModelInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLRunningNode::GetLiveParametersInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRunningNodePinNames::LiveParametersInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLRunningNode::GetRunInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRunningNodePinNames::RunInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

// pin access helpers : outputs
//
UEdGraphPin* UInteractMLRunningNode::GetLabelOutputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLRunningNodePinNames::LabelOutputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}



// runtime node operation functionality hookup
//
void UInteractMLRunningNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );
	
	//generate node disambiguation/context
	FString NodeID = NodeGuid.ToString( EGuidFormats::Digits );
	
	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : data
	UEdGraphPin* MainModelPin = GetModelInputPin();
	UEdGraphPin* MainLiveParametersPin = GetLiveParametersInputPin();
	UEdGraphPin* MainRunPin = GetRunInputPin();
	//output pins : exec (execution continues)
	UEdGraphPin* MainThenPin = FindPin( UEdGraphSchema_K2::PN_Then );	
	//output pins : data
	UEdGraphPin* MainLabelOutputPin = GetLabelOutputPin();

	//internal model run fn
	UFunction* RunFn = FindModelRunFunction();
	UK2Node_CallFunction* CallRunFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallRunFn->SetFromFunction( RunFn );
	CallRunFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallRunFn, this);
	//run fn pins
	UEdGraphPin* RunFnExecPin = CallRunFn->GetExecPin();
	UEdGraphPin* RunFnThenPin = CallRunFn->GetThenPin();
	UEdGraphPin* RunFnActorPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::ActorPinName );
	UEdGraphPin* RunFnModelPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::ModelPinName );
	UEdGraphPin* RunFnLiveParametersPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::LiveParametersPinName );
	UEdGraphPin* RunFnRunPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::RunPinName );
	UEdGraphPin* RunFnNodeIDPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::NodeIDPinName );

	//label pins
	if (LabelType!=nullptr)
	{
		//composite label operation (passed struct filled in)
		UEdGraphPin* RunFnLabelDataPin = CallRunFn->FindPinChecked(FInteractMLRunningNodeRunModelPinNames::LabelDataPinName);
		CompilerContext.MovePinLinksToIntermediate(*MainLabelOutputPin, *RunFnLabelDataPin);
		//force generic struct to be the known struct type for passing
		RunFnLabelDataPin->PinType = MainLabelOutputPin->PinType;
		//there is a type pin to set up too
		UEdGraphPin* RunFnLabelTypePin = CallRunFn->FindPinChecked(FInteractMLRunningNodeRunModelPinNames::LabelTypePinName);
		RunFnLabelTypePin->DefaultObject = LabelType;
	}
	else
	{
		//simple label operation (result returned)
		UEdGraphPin* RunFnResultPin = CallRunFn->GetReturnValuePin();
		CompilerContext.MovePinLinksToIntermediate(*MainLabelOutputPin, *RunFnResultPin);
	}
	
	//chain functionality together
	CompilerContext.MovePinLinksToIntermediate( *MainExecPin, *RunFnExecPin );
	CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *RunFnThenPin );
	
	//hook up rest of run fn pins
	ConnectContextActor(CompilerContext, SourceGraph, RunFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainModelPin, *RunFnModelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLiveParametersPin, *RunFnLiveParametersPin);
	CompilerContext.MovePinLinksToIntermediate(*MainRunPin, *RunFnRunPin);
	RunFnNodeIDPin->DefaultValue = NodeID;

	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();
}

// locate function used to run the model
//
UFunction* UInteractMLRunningNode::FindModelRunFunction() const
{
	FName record_function_name;
	if (LabelType)
	{
		//composite label	
		record_function_name = FInteractMLRunningNodeFunctionNames::RunModelCompositeFunctionName;
	}
	else
	{
		//simple label
		record_function_name = FInteractMLRunningNodeFunctionNames::RunModelSimpleFunctionName;
	}
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( record_function_name );
}



#undef LOCTEXT_NAMESPACE
