//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLModelNode.h"

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

namespace FInteractMLModelNodePinNames
{
	//in
	static const FName DataPathInputPinName("Data Path");
	static const FName LiveParametersInputPinName("Live Parameters");
	static const FName RunInputPinName("Run");
	static const FName TrainingSetInputPinName("Training Set");
	static const FName TrainInputPinName("Train");
	static const FName ResetInputPinName("Reset");
	//out
	static const FName TrainedOutputPinName("Trained");
	static const FName LabelOutputPinName("Label");
}  	
namespace FInteractMLModelNodeFunctionNames
{
	static const FName RunModelName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RunModel));
	static const FName TrainModelName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, TrainModel));
}
//UInteractMLBlueprintLibrary::GetModel_*(...)
namespace FModelNodeModelAccessPinNames
{
	static const FName ActorPinName("Actor");
	static const FName DataPathPinName("DataPath");
	static const FName NodeIDPinName("NodeID");
}
//UInteractMLBlueprintLibrary::RunModel(...)
namespace FModelNodeRunModelPinNames
{
	static const FName ActorPinName( "Actor" );
	static const FName ModelPinName("Model");
	static const FName LiveParametersPinName("Parameters");
	static const FName RunPinName("Run");
	static const FName NodeIDPinName("NodeID");
}
//UInteractMLBlueprintLibrary::TrainModel(...)
namespace FModelNodeTrainModelPinNames
{
	static const FName ModelPinName("Model");
	static const FName TrainingSetPinName("TrainingSet");
	static const FName TrainPinName("Train");
	static const FName ResetPinName("Reset");
	static const FName NodeIDPinName("NodeID");
}

/////////////////////////////////// HELPERS /////////////////////////////////////



//////////////////////////////// MODEL NODE (BASE) CLASS ////////////////////////////////////

// basic node properties
//
FText UInteractMLModelNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString title = FText::Format(LOCTEXT("ModelNodeTitle", "{0} Robot"), { GetModelName() }).ToString();

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
			title.Append(TEXT("\n"));
			title.Append( LOCTEXT("ModelNodeSubTitle", "Machine Learning System").ToString() );
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
FText UInteractMLModelNode::GetTooltipText() const
{
	return GetModelTooltip();
}
FText UInteractMLModelNode::GetMenuCategory() const
{
	return InteractMLConstants::NodeMenuCategory;
}
FLinearColor UInteractMLModelNode::GetNodeTitleColor() const
{
	return InteractMLConstants::NodeTitleColour.ReinterpretAsLinear(); 
}


// put node in blueprint menu
//
void UInteractMLModelNode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions( ActionRegistrar );
#if false
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
#endif
}

// custom pins
//
void UInteractMLModelNode::AllocateDefaultPins()
{
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	//---- Inputs ----

	// Target actor (needed for context)
	UEdGraphPin* actor_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, AActor::StaticClass(), InteractMLConstants::BlueprintNodeActorPinName);
	actor_pin->PinToolTip = InteractMLConstants::BlueprintNodeActorPinTooltip.ToString();

	// Which data file to persist training data?	
	UEdGraphPin* data_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, nullptr, FInteractMLModelNodePinNames::DataPathInputPinName);
	data_pin->PinToolTip = LOCTEXT("ModelNodeDataPinTooltip", "Path (optional) and Name to load/save model data.").ToString();

	// parameters to record
	UEdGraphPin* liveparams_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParameters>::Get(), FInteractMLModelNodePinNames::LiveParametersInputPinName);
	liveparams_pin->PinToolTip = LOCTEXT("ModelNodeLiveParamsPinTooltip", "The live parameters that the model will be running against.").ToString();

	// enable run
	UEdGraphPin* run_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLModelNodePinNames::RunInputPinName);
	run_pin->PinToolTip = LOCTEXT("ModelNodeRunPinTooltip", "Set this to run the model against the current set of input parameters.").ToString();

	// training set to learn from
	UEdGraphPin* trainingset_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteractMLTrainingSet::StaticClass(), FInteractMLModelNodePinNames::TrainingSetInputPinName);
	trainingset_pin->PinToolTip = LOCTEXT("ModelNodeTrainingSetPinTooltip", "The training set for the model to be trained with.").ToString();
	
	// enable train
	UEdGraphPin* train_pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLModelNodePinNames::TrainInputPinName);
	train_pin->PinToolTip = LOCTEXT("ModelNodeTrainPinTooltip", "Set this to train the model using the supplied training set.").ToString();
	
	// perform reset
	UEdGraphPin* reset_pin = CreatePin( EGPD_Input, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLModelNodePinNames::ResetInputPinName );
	reset_pin->PinToolTip = LOCTEXT( "ModelNodeResetPinTooltip", "Set this to clear out the model leaving it un-trained." ).ToString();

	//---- Outputs ----

	//trained pin
	UEdGraphPin* trained_pin = CreatePin( EGPD_Output, UEdGraphSchema_K2::PC_Boolean, nullptr, FInteractMLModelNodePinNames::TrainedOutputPinName );
	trained_pin->PinToolTip = LOCTEXT( "ModelNodeTrainedPinTooltip", "Indicated whether the current model trained or not." ).ToString();
	// label pin
	UEdGraphPin* label_pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Float, nullptr, FInteractMLModelNodePinNames::LabelOutputPinName);
	label_pin->PinToolTip = LOCTEXT("ModelNodeLabelPinTooltip", "Result of running the model on the input parameters.").ToString();
	
	Super::AllocateDefaultPins();
}


// pin access helpers : inputs
//
UEdGraphPin* UInteractMLModelNode::GetActorInputPin() const
{
	UEdGraphPin* Pin = FindPin(InteractMLConstants::BlueprintNodeActorPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLModelNode::GetDataPathInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLModelNodePinNames::DataPathInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLModelNode::GetLiveParametersInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLModelNodePinNames::LiveParametersInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLModelNode::GetRunInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLModelNodePinNames::RunInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLModelNode::GetTrainingSetInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLModelNodePinNames::TrainingSetInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}
UEdGraphPin* UInteractMLModelNode::GetTrainInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLModelNodePinNames::TrainInputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Input );
	return Pin;
}
UEdGraphPin* UInteractMLModelNode::GetResetInputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLModelNodePinNames::ResetInputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Input );
	return Pin;
}

// pin access helpers : outputs
//
UEdGraphPin* UInteractMLModelNode::GetTrainedOutputPin() const
{
	UEdGraphPin* Pin = FindPin( FInteractMLModelNodePinNames::TrainedOutputPinName );
	check( Pin == NULL || Pin->Direction == EGPD_Output );
	return Pin;
}
UEdGraphPin* UInteractMLModelNode::GetLabelOutputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLModelNodePinNames::LabelOutputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}



// runtime node operation functionality hookup
//
void UInteractMLModelNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );
	
	CompilerContext.MessageLog.Warning(TEXT("DEPRECATED: Please replace @@ with 'Teach The Machine' node or 'Machine Learning Robot' node."), this);

	//generate node disambiguation/context
	FString NodeID = NodeGuid.ToString( EGuidFormats::Digits );
	
	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : data
	UEdGraphPin* MainActorPin = GetActorInputPin();
	UEdGraphPin* MainDataPathPin = GetDataPathInputPin();
	UEdGraphPin* MainLiveParametersPin = GetLiveParametersInputPin();
	UEdGraphPin* MainRunPin = GetRunInputPin();
	UEdGraphPin* MainTrainingSetPin = GetTrainingSetInputPin();
	UEdGraphPin* MainTrainPin = GetTrainInputPin();
	UEdGraphPin* MainResetPin = GetResetInputPin();
	//output pins : exec (execution continues)
	UEdGraphPin* MainThenPin = FindPin( UEdGraphSchema_K2::PN_Then );	
	//output pins : data
	UEdGraphPin* MainTrainedOutputPin = GetTrainedOutputPin();
	UEdGraphPin* MainLabelOutputPin = GetLabelOutputPin();

	//internal model accessor
	UFunction* ModelFn = FindModelAccessFunction();
	UK2Node_CallFunction* CallModelFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallModelFn->SetFromFunction( ModelFn );
	CallModelFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallModelFn, this);
	//model fn pins
	UEdGraphPin* ModelFnExecPin = CallModelFn->GetExecPin();
	UEdGraphPin* ModelFnThenPin = CallModelFn->GetThenPin();
	UEdGraphPin* ModelFnResultPin = CallModelFn->GetReturnValuePin();
	UEdGraphPin* ModelFnActorPin = CallModelFn->FindPinChecked( FModelNodeModelAccessPinNames::ActorPinName );
	UEdGraphPin* ModelFnDataPathPin = CallModelFn->FindPinChecked( FModelNodeModelAccessPinNames::DataPathPinName );
	UEdGraphPin* ModelFnNodeIDPin = CallModelFn->FindPinChecked( FModelNodeModelAccessPinNames::NodeIDPinName );

	//internal model run fn
	UFunction* RunFn = FindModelRunFunction();
	UK2Node_CallFunction* CallRunFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallRunFn->SetFromFunction( RunFn );
	CallRunFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallRunFn, this);
	//run fn pins
	UEdGraphPin* RunFnExecPin = CallRunFn->GetExecPin();
	UEdGraphPin* RunFnThenPin = CallRunFn->GetThenPin();
	UEdGraphPin* RunFnResultPin = CallRunFn->GetReturnValuePin();
	UEdGraphPin* RunFnActorPin = CallRunFn->FindPinChecked( FModelNodeRunModelPinNames::ActorPinName );
	UEdGraphPin* RunFnModelPin = CallRunFn->FindPinChecked( FModelNodeRunModelPinNames::ModelPinName );
	UEdGraphPin* RunFnLiveParametersPin = CallRunFn->FindPinChecked( FModelNodeRunModelPinNames::LiveParametersPinName );
	UEdGraphPin* RunFnRunPin = CallRunFn->FindPinChecked( FModelNodeRunModelPinNames::RunPinName );
	UEdGraphPin* RunFnNodeIDPin = CallRunFn->FindPinChecked( FModelNodeRunModelPinNames::NodeIDPinName );
	
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
	UEdGraphPin* TrainFnModelPin = CallTrainFn->FindPinChecked( FModelNodeTrainModelPinNames::ModelPinName );
	UEdGraphPin* TrainFnTrainingSetPin = CallTrainFn->FindPinChecked( FModelNodeTrainModelPinNames::TrainingSetPinName);
	UEdGraphPin* TrainFnTrainPin = CallTrainFn->FindPinChecked( FModelNodeTrainModelPinNames::TrainPinName );
	UEdGraphPin* TrainFnResetPin = CallTrainFn->FindPinChecked( FModelNodeTrainModelPinNames::ResetPinName );
	UEdGraphPin* TrainFnNodeIDPin = CallTrainFn->FindPinChecked( FModelNodeTrainModelPinNames::NodeIDPinName );

	//chain execution pins (model access, then run, then training)
	CompilerContext.MovePinLinksToIntermediate( *MainExecPin, *ModelFnExecPin );
	ModelFnThenPin->MakeLinkTo( RunFnExecPin );
	RunFnThenPin->MakeLinkTo( TrainFnExecPin );
	CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *TrainFnThenPin );

	//hook up model access pins
	CompilerContext.MovePinLinksToIntermediate(*MainActorPin, *ModelFnActorPin);
	CompilerContext.MovePinLinksToIntermediate(*MainDataPathPin, *ModelFnDataPathPin);
	ModelFnNodeIDPin->DefaultValue = NodeID;
	
	//hook up run fn pins
	ModelFnResultPin->MakeLinkTo( RunFnModelPin );
	CompilerContext.CopyPinLinksToIntermediate( *ModelFnActorPin, *RunFnActorPin );	//this run fn pin wants same incoming connection as model fn
	CompilerContext.MovePinLinksToIntermediate(*MainLiveParametersPin, *RunFnLiveParametersPin);
	CompilerContext.MovePinLinksToIntermediate(*MainRunPin, *RunFnRunPin);
	RunFnNodeIDPin->DefaultValue = NodeID;
	CompilerContext.MovePinLinksToIntermediate(*MainLabelOutputPin, *RunFnResultPin);

	//hook up train fn pins
	ModelFnResultPin->MakeLinkTo(TrainFnModelPin);
	CompilerContext.MovePinLinksToIntermediate(*MainTrainingSetPin, *TrainFnTrainingSetPin);
	CompilerContext.MovePinLinksToIntermediate(*MainTrainPin, *TrainFnTrainPin);
	CompilerContext.MovePinLinksToIntermediate(*MainResetPin, *TrainFnResetPin);
	TrainFnNodeIDPin->DefaultValue = NodeID;
	CompilerContext.MovePinLinksToIntermediate( *MainTrainedOutputPin, *TrainFnResultPin );

	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();
}

// locate function used to run the model
//
UFunction* UInteractMLModelNode::FindModelRunFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( FInteractMLModelNodeFunctionNames::RunModelName );
}

// locate function used to train the model
//
UFunction* UInteractMLModelNode::FindModelTrainFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( FInteractMLModelNodeFunctionNames::TrainModelName );
}

// locate function to access the model instance
//
UFunction* UInteractMLModelNode::FindModelAccessFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( GetModelAccessFunctionName() );
}



#undef LOCTEXT_NAMESPACE
