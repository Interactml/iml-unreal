//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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
#include "K2Node_ExecutionSequence.h" //sequence intermediate
#include "K2Node_IfThenElse.h" //branch intermediate

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
	static const FName LiveParametersInputPinName("Parameters");
	static const FName RunInputPinName("Run");
	//out
	static const FName LabelOutputPinName("Label");
	//out (async only)
	static const FName RunningOutputPinName("Running");
	static const FName CompletedOutputPinName("Completed");
}  	
namespace FInteractMLRunningNodeFunctionNames
{
	static const FName RunModelSimpleFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RunModelSimple));
	static const FName RunModelCompositeFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RunModelComposite));
	static const FName RunModelSimpleAsyncFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RunModelSimpleAsync));
	static const FName RunModelCompositeAsyncFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RunModelCompositeAsync));
}
//UInteractMLBlueprintLibrary::RunModel*(...)
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
	//async only
	static const FName RunningPinName("Running");
	static const FName CompletedPinName("Completed");
}

/////////////////////////////////// HELPERS /////////////////////////////////////


//////////////////////////////// MODEL NODE (BASE) CLASS ////////////////////////////////////

// node title
//
FText UInteractMLRunningNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString title = LOCTEXT("RunningNodeTitle", "Machine Learning Robot").ToString();

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
			title.Append(TEXT("\n"));
			if (bBackgroundOperation)
			{
				title.Append(LOCTEXT("RunningNodeSubTitleAsync", "(recognise parameters in the background)").ToString());
			}
			else
			{
				title.Append(LOCTEXT("RunningNodeSubTitle", "Use a model to recognise parameters").ToString());
			}
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

// node tooltip
//
FText UInteractMLRunningNode::GetTooltipText() const
{
	return LOCTEXT("RunningNodeTooltip", "Runs a machine learning model on live parameters to find a trained output");
}

// monitor property changes that may invalidate node structure (e.g. exposed pins or title)
//
void UInteractMLRunningNode::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	const FName PropertyName = e.GetPropertyName();
	
	//label and background operation both affect pins	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UInteractMLRunningNode, LabelType)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(UInteractMLRunningNode, bBackgroundOperation))
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
	liveparams_pin->PinFriendlyName = LOCTEXT("ParameterInputPinName", "Live Parameters");
	
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
		label_pin->PinFriendlyName = LOCTEXT("LabelOutputPinName", "Output");
	}
	else
	{
		//simple numerical label
		UEdGraphPin* label_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Float, nullptr, FInteractMLRunningNodePinNames::LabelOutputPinName);
		label_pin->PinToolTip = LOCTEXT("RecordingNodeSimpleNumericLabelPinTooltip", "Result of running the model on the input parameters.").ToString();
		label_pin->PinFriendlyName = LOCTEXT("LabelOutputPinName", "Output");
	}

	//optional pins (async operation)
	if (bBackgroundOperation)
	{
		//running pin
		UEdGraphPin* running_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLRunningNodePinNames::RunningOutputPinName);
		running_pin->PinToolTip = LOCTEXT("RunningNodeTrainingPinTooltip", "Indicates that a background run operation is in progress.").ToString();
		
		//completion pin
		UEdGraphPin* completed_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, nullptr, FInteractMLRunningNodePinNames::CompletedOutputPinName);
		completed_pin->PinToolTip = LOCTEXT("RunningNodeCompletedPinTooltip", "Executes briefly when a run has completed.").ToString();
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
UEdGraphPin* UInteractMLRunningNode::GetRunningOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLRunningNodePinNames::RunningOutputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Output );
	return Pin;
}
UEdGraphPin* UInteractMLRunningNode::GetCompletedOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLRunningNodePinNames::CompletedOutputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Output );
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
	//output pins : async only
	UEdGraphPin* MainRunningOutputPin = nullptr; 
	UEdGraphPin* MainCompletedOutputPin = nullptr; 
	if (bBackgroundOperation)
	{
		MainRunningOutputPin = GetRunningOutputPin();
		MainCompletedOutputPin = GetCompletedOutputPin();
	}
	
	//internal model run fn
	UFunction* RunFn = FindModelRunFunction();
	UK2Node_CallFunction* CallRunFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallRunFn->SetFromFunction( RunFn );
	CallRunFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallRunFn, this);
	//run fn pins
	UEdGraphPin* RunFnExecPin = CallRunFn->GetExecPin();
	UEdGraphPin* RunFnThenPin = CallRunFn->GetThenPin();
	UEdGraphPin* RunFnRunningPin = nullptr;
	UEdGraphPin* RunFnCompletedPin = nullptr;
	UEdGraphPin* RunFnActorPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::ActorPinName );
	UEdGraphPin* RunFnModelPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::ModelPinName );
	UEdGraphPin* RunFnLiveParametersPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::LiveParametersPinName );
	UEdGraphPin* RunFnRunPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::RunPinName );
	UEdGraphPin* RunFnNodeIDPin = CallRunFn->FindPinChecked( FInteractMLRunningNodeRunModelPinNames::NodeIDPinName );
	if (bBackgroundOperation)
	{
		RunFnRunningPin = CallRunFn->FindPinChecked(FInteractMLRunningNodeRunModelPinNames::RunningPinName);
		RunFnCompletedPin = CallRunFn->FindPinChecked(FInteractMLRunningNodeRunModelPinNames::CompletedPinName);
	}
	
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
	UEdGraphPin* SequenceSecondExePin = nullptr;
	if (bBackgroundOperation)
	{
		//insert sequence node to drive the two exec outputs (defaults to two output pins)
		//NOTE: this is needed to drive the main then as well as the optional completed exec pin (via the branch below)
		UK2Node_ExecutionSequence* SequenceNode = CompilerContext.SpawnIntermediateNode<UK2Node_ExecutionSequence>(this, SourceGraph);
		SequenceNode->AllocateDefaultPins();
		UEdGraphPin* SequenceInputExePin = SequenceNode->GetExecPin();
		UEdGraphPin* SequenceFirstExePin = SequenceNode->GetThenPinGivenIndex(0);
		SequenceSecondExePin = SequenceNode->GetThenPinGivenIndex(1);
		
		//hook up
		RunFnThenPin->MakeLinkTo( SequenceInputExePin );
		CompilerContext.MovePinLinksToIntermediate(*MainThenPin, *SequenceFirstExePin);
	}
	else
	{
		//straight through
		CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *RunFnThenPin );
	}
	
	//hook up rest of run fn pins
	ConnectContextActor(CompilerContext, SourceGraph, RunFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainModelPin, *RunFnModelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainLiveParametersPin, *RunFnLiveParametersPin);
	CompilerContext.MovePinLinksToIntermediate(*MainRunPin, *RunFnRunPin);
	RunFnNodeIDPin->DefaultValue = NodeID;

	//hook up and handle extra running outputs
	if (bBackgroundOperation)
	{
		//running indicator
		CompilerContext.MovePinLinksToIntermediate(*MainRunningOutputPin, *RunFnRunningPin);
		
		//need branch to operate the completed exec output pin
		//NOTE: This causes the completed exec output to execute when the run function returns true from the Completed (bool) output (pulses a single frame on completion)
		UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
		BranchNode->AllocateDefaultPins();
		
		//hook up
		SequenceSecondExePin->MakeLinkTo( BranchNode->GetExecPin() );
		RunFnCompletedPin->MakeLinkTo( BranchNode->GetConditionPin() );
		CompilerContext.MovePinLinksToIntermediate(*MainCompletedOutputPin, *BranchNode->GetThenPin());
	}
	
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
		if (bBackgroundOperation)
		{
			record_function_name = FInteractMLRunningNodeFunctionNames::RunModelCompositeAsyncFunctionName;
		}
		else
		{
			record_function_name = FInteractMLRunningNodeFunctionNames::RunModelCompositeFunctionName;
		}
	}
	else
	{
		//simple label
		if (bBackgroundOperation)
		{
			record_function_name = FInteractMLRunningNodeFunctionNames::RunModelSimpleAsyncFunctionName;
		}
		else
		{
			record_function_name = FInteractMLRunningNodeFunctionNames::RunModelSimpleFunctionName;
		}
	}
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( record_function_name );
}



#undef LOCTEXT_NAMESPACE
