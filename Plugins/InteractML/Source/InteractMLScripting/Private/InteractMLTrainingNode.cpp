//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLTrainingNode.h"

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
namespace FInteractMLTrainingNodePinNames
{
	//in
	static const FName ModelInputPinName("Model");
	static const FName TrainingSetInputPinName("Training Set");
	static const FName TrainInputPinName("Train");
	static const FName ResetInputPinName("Reset");
	//out
	static const FName TrainedOutputPinName("Trained");
	//async only
	static const FName TrainingOutputPinName("Training");
	static const FName CompletedOutputPinName("Completed");
}  	
namespace FInteractMLTrainingNodeFunctionNames
{
	static const FName TrainModelFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, TrainModel));
	static const FName TrainModelAsyncFunctionName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, TrainModelAsync));
}
//UInteractMLBlueprintLibrary::TrainModel(...)
namespace FInteractMLTrainingNodeTrainModelPinNames
{
	static const FName ActorPinName("Actor");
	static const FName ModelPinName("Model");
	static const FName TrainingSetPinName("TrainingSet");
	static const FName TrainPinName("Train");
	static const FName ResetPinName("Reset");
	static const FName NodeIDPinName("NodeID");
	//async only
	static const FName TrainingPinName("Training");
	static const FName CompletedPinName("Completed");
}

/////////////////////////////////// HELPERS /////////////////////////////////////


//////////////////////////////// TRAINING NODE CLASS ////////////////////////////////////

// node title
//
FText UInteractMLTrainingNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString title = LOCTEXT("TrainingNodeTitle", "Teach The Machine").ToString();

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
			if (bBackgroundOperation)
			{
				title.Append(TEXT("\n"));
				title.Append(LOCTEXT("TrainingNodeSubTitleAsync", "(in the background)").ToString());
			}
			else
			{
				title.Append(TEXT("\n"));
				title.Append(LOCTEXT("TrainingNodeSubTitle", "Train a machine learning model").ToString());
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
FText UInteractMLTrainingNode::GetTooltipText() const
{
	return LOCTEXT("TrainingNodeTooltip", "Train a machine learning model using examples in a training set");
}

// monitor property changes that may invalidate node structure (e.g. exposed pins or title)
//
void UInteractMLTrainingNode::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	const FName PropertyName = e.GetPropertyName();

	//background operation affects pins	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UInteractMLTrainingNode, bBackgroundOperation))
	{
		//rebuild because async mode changed
		ReconstructNode();
	}
	
	Super::PostEditChangeProperty(e);
}

// custom pins
//
void UInteractMLTrainingNode::AllocateDefaultPins()
{
	//handle context actor pin
	Super::AllocateDefaultPins();

	//---- Inputs ----

	// Which trainging set to record into?	
	UEdGraphPin* model_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteractMLModel::StaticClass(), FInteractMLTrainingNodePinNames::ModelInputPinName );
	model_pin->PinToolTip = LOCTEXT("TrainingNodeModelTooltip", "Model to be trained.").ToString();
		
	// training set to learn from
	UEdGraphPin* trainingset_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteractMLTrainingSet::StaticClass(), FInteractMLTrainingNodePinNames::TrainingSetInputPinName);
	trainingset_pin->PinToolTip = LOCTEXT("TrainingNodeTrainingSetPinTooltip", "The training set for the model to be trained with.").ToString();
	
	// enable train
	UEdGraphPin* train_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLTrainingNodePinNames::TrainInputPinName);
	train_pin->PinToolTip = LOCTEXT("TrainingNodeTrainPinTooltip", "Set this to train the model using the supplied training set.").ToString();
	
	// perform reset
	UEdGraphPin* reset_pin = CreatePin( EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLTrainingNodePinNames::ResetInputPinName );
	reset_pin->PinToolTip = LOCTEXT( "TrainingNodeResetPinTooltip", "Set this to clear out the model leaving it un-trained." ).ToString();

	//---- Outputs ----

	//trained pin
	UEdGraphPin* trained_pin = CreatePin( EGPD_Output, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLTrainingNodePinNames::TrainedOutputPinName );
	trained_pin->PinToolTip = LOCTEXT( "TrainingNodeTrainedPinTooltip", "Indicated whether the current model is trained or not." ).ToString();
	
	//optional pins (async operation)
	if (bBackgroundOperation)
	{
		//training pin
		UEdGraphPin* training_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLTrainingNodePinNames::TrainingOutputPinName);
		training_pin->PinToolTip = LOCTEXT("TrainingNodeTrainingPinTooltip", "Indicates that a background training operation is in progress.").ToString();

		//completion pin
		UEdGraphPin* completed_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, nullptr, FInteractMLTrainingNodePinNames::CompletedOutputPinName);
		completed_pin->PinToolTip = LOCTEXT("TrainingNodeCompletedPinTooltip", "Executes briefly when training has completed.").ToString();
	}
}


// pin access helpers : inputs
//
UEdGraphPin* UInteractMLTrainingNode::GetModelInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingNodePinNames::ModelInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLTrainingNode::GetTrainingSetInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLTrainingNodePinNames::TrainingSetInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLTrainingNode::GetTrainInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLTrainingNodePinNames::TrainInputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Input );
	return Pin;
}
UEdGraphPin* UInteractMLTrainingNode::GetResetInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLTrainingNodePinNames::ResetInputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Input );
	return Pin;
}

// pin access helpers : outputs
//
UEdGraphPin* UInteractMLTrainingNode::GetTrainedOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLTrainingNodePinNames::TrainedOutputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Output );
	return Pin;
}
UEdGraphPin* UInteractMLTrainingNode::GetTrainingOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLTrainingNodePinNames::TrainingOutputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Output );
	return Pin;
}
UEdGraphPin* UInteractMLTrainingNode::GetCompletedOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLTrainingNodePinNames::CompletedOutputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Output );
	return Pin;
}

// runtime node operation functionality hookup
//
void UInteractMLTrainingNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );
	
	//generate node disambiguation/context
	FString NodeID = NodeGuid.ToString( EGuidFormats::Digits );
	
	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : data
	UEdGraphPin* MainModelPin = GetModelInputPin();
	UEdGraphPin* MainTrainingSetPin = GetTrainingSetInputPin();
	UEdGraphPin* MainTrainPin = GetTrainInputPin();
	UEdGraphPin* MainResetPin = GetResetInputPin();
	//output pins : exec (execution continues)
	UEdGraphPin* MainThenPin = FindPin( UEdGraphSchema_K2::PN_Then );	
	//output pins : data
	UEdGraphPin* MainTrainedOutputPin = GetTrainedOutputPin();
	//output pins : async only
	UEdGraphPin* MainTrainingOutputPin = nullptr; 
	UEdGraphPin* MainCompletedOutputPin = nullptr; 
	if (bBackgroundOperation)
	{
		MainTrainingOutputPin = GetTrainingOutputPin();
		MainCompletedOutputPin = GetCompletedOutputPin();
	}

	//internal model training fn
	UFunction* TrainFn = FindModelTrainFunction();
	UK2Node_CallFunction* CallTrainFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallTrainFn->SetFromFunction( TrainFn );
	CallTrainFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallTrainFn, this);
	//training fn pins
	UEdGraphPin* TrainFnExecPin = CallTrainFn->GetExecPin();
	UEdGraphPin* TrainFnThenPin = CallTrainFn->GetThenPin();
	UEdGraphPin* TrainFnResultPin = CallTrainFn->GetReturnValuePin();
	UEdGraphPin* TrainFnTrainingPin = nullptr;
	UEdGraphPin* TrainFnCompletedPin = nullptr;
	UEdGraphPin* TrainFnActorPin = CallTrainFn->FindPinChecked( FInteractMLTrainingNodeTrainModelPinNames::ActorPinName );
	UEdGraphPin* TrainFnModelPin = CallTrainFn->FindPinChecked( FInteractMLTrainingNodeTrainModelPinNames::ModelPinName );
	UEdGraphPin* TrainFnTrainingSetPin = CallTrainFn->FindPinChecked( FInteractMLTrainingNodeTrainModelPinNames::TrainingSetPinName);
	UEdGraphPin* TrainFnTrainPin = CallTrainFn->FindPinChecked( FInteractMLTrainingNodeTrainModelPinNames::TrainPinName );
	UEdGraphPin* TrainFnResetPin = CallTrainFn->FindPinChecked( FInteractMLTrainingNodeTrainModelPinNames::ResetPinName );
	UEdGraphPin* TrainFnNodeIDPin = CallTrainFn->FindPinChecked( FInteractMLTrainingNodeTrainModelPinNames::NodeIDPinName );
	if (bBackgroundOperation)
	{
		TrainFnTrainingPin = CallTrainFn->FindPinChecked(FInteractMLTrainingNodeTrainModelPinNames::TrainingPinName);
		TrainFnCompletedPin = CallTrainFn->FindPinChecked(FInteractMLTrainingNodeTrainModelPinNames::CompletedPinName);
	}

	//chain functionality together
	CompilerContext.MovePinLinksToIntermediate(*MainExecPin, *TrainFnExecPin);
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
		TrainFnThenPin->MakeLinkTo( SequenceInputExePin );
		CompilerContext.MovePinLinksToIntermediate(*MainThenPin, *SequenceFirstExePin);
	}
	else
	{
		//straight through
		CompilerContext.MovePinLinksToIntermediate(*MainThenPin, *TrainFnThenPin);
	}
	
	//hook up train fn pins
	ConnectContextActor(CompilerContext, SourceGraph, TrainFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainModelPin, *TrainFnModelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainTrainingSetPin, *TrainFnTrainingSetPin);
	CompilerContext.MovePinLinksToIntermediate(*MainTrainPin, *TrainFnTrainPin);
	CompilerContext.MovePinLinksToIntermediate(*MainResetPin, *TrainFnResetPin);
	TrainFnNodeIDPin->DefaultValue = NodeID;
	CompilerContext.MovePinLinksToIntermediate( *MainTrainedOutputPin, *TrainFnResultPin );

	//hook up and handle extra training outputs
	if (bBackgroundOperation)
	{
		//training indicator
		CompilerContext.MovePinLinksToIntermediate(*MainTrainingOutputPin, *TrainFnTrainingPin);

		//need branch to operate the completed exec output pin
		//NOTE: This causes the completed exec output to execute when the training function returns true from the Completed (bool) output (pulses a single frame on completion)
		UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
		BranchNode->AllocateDefaultPins();
		
		//hook up
		SequenceSecondExePin->MakeLinkTo( BranchNode->GetExecPin() );
		TrainFnCompletedPin->MakeLinkTo( BranchNode->GetConditionPin() );
		CompilerContext.MovePinLinksToIntermediate(*MainCompletedOutputPin, *BranchNode->GetThenPin());
	}

	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();
}

// locate function used to train the model
//
UFunction* UInteractMLTrainingNode::FindModelTrainFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	if (bBackgroundOperation)
	{
		return LibraryClass->FindFunctionByName(FInteractMLTrainingNodeFunctionNames::TrainModelAsyncFunctionName);
	}
	else
	{
		return LibraryClass->FindFunctionByName(FInteractMLTrainingNodeFunctionNames::TrainModelFunctionName);
	}
}

#undef LOCTEXT_NAMESPACE
