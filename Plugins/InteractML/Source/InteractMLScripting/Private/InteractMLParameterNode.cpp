//----
// InteractML - University Of Arts London
//----

#include "InteractMLParameterNode.h"

//unreal
#include "BlueprintActionDatabaseRegistrar.h" //FBlueprintActionDatabaseRegistrar
#include "BlueprintNodeSpawner.h" //UBlueprintNodeSpawner
#include "EdGraphSchema_K2.h" //UEdGraphSchema_K2
#include "KismetCompiler.h" //FKismetCompilerContext
#include "K2Node_CallFunction.h" //UK2Node_Function
#include "Engine/SimpleConstructionScript.h" //USimpleConstructionScript
#include "BlueprintEditorUtils.h" //MarkBlueprintAsStructurallyModified

//module
#include "InteractMLParameters.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

static FColor cInteractMLPrimaryColour(115, 100, 205); //IML Purple); // 58, 59, 91);

// LOCAL CLASSES & TYPES


// info about types of compatible input parameters
//
struct FInputParameterInfo
{
	FName Type;
	UScriptStruct* Struct;
	int NumFloats;

	FInputParameterInfo(FName type, int num_floats)
		: Type(type)
		, Struct(nullptr)
		, NumFloats(num_floats)
	{}
	FInputParameterInfo(FName type, UScriptStruct* struct_type, int num_floats)
		: Type(type)
		, Struct(struct_type)
		, NumFloats(num_floats)
	{}
};
static TArray<FInputParameterInfo>& GetInputParameterInfoList()
{
	static TArray<FInputParameterInfo> list;
	if(list.Num()==0)
	{
		list.Add(FInputParameterInfo(UEdGraphSchema_K2::PC_Int, 1));
		list.Add(FInputParameterInfo(UEdGraphSchema_K2::PC_Float, 1));
		list.Add(FInputParameterInfo(UEdGraphSchema_K2::PC_Boolean, 1));
		list.Add(FInputParameterInfo(UEdGraphSchema_K2::PC_Struct, TBaseStructure<FVector>::Get(), 3));
		list.Add(FInputParameterInfo(UEdGraphSchema_K2::PC_Struct, TBaseStructure<FColor>::Get(), 3));
	};
	return list;
}
static bool IsCompatibleInputParameterType(FName type)
{
	static TArray<FInputParameterInfo>& list = GetInputParameterInfoList();
	for (int i = 0; i < list.Num(); i++)
	{
		if (list[i].Type == type)
		{
			return true;
		}
	}
	return false;
}
static FInputParameterInfo& GetInputParameterInfo(FName type)
{
	static TArray<FInputParameterInfo>& list = GetInputParameterInfoList();
	for (int i = 0; i < list.Num(); i++)
	{
		if (list[i].Type == type)
		{
			return list[i];
		}
	}
	static FInputParameterInfo none(UEdGraphSchema_K2::PC_Wildcard, 0);
	return none;
}

// name constants
//

namespace FInteractMLParameterNodePinNames
{
	//in
	static const FName TargetPinName("Actor");
	//out
	static const FName OutputPinName("Parameters");
}  	


// basic node properties
//
FText UInteractMLParameterNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText node_name = LOCTEXT("InteractMLParameterNode_Title", "Collect All The Things!");

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
		{
			FString title = node_name.ToString();
			int param_count = CountParameters();
			int float_count = CountFloats();
			title.Append(TEXT("\n"));
			title.Append( FText::Format(LOCTEXT("InteractMLParameterNode_SubTitle", "{0} parameter(s) so far ({1} floats)"), param_count, float_count ).ToString() );
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
FText UInteractMLParameterNode::GetTooltipText() const
{
	return LOCTEXT("InteractMLParameterNode_Tooltip", "Connect all the parameters to be used as training inputs");
}
FText UInteractMLParameterNode::GetMenuCategory() const
{
	return LOCTEXT("InteractMLParameterNode_MenuCategory", "InteractML");
}
FLinearColor UInteractMLParameterNode::GetNodeTitleColor() const
{
	return cInteractMLPrimaryColour.ReinterpretAsLinear(); 
}


// how many valid parameters does this collection have?
//
int UInteractMLParameterNode::CountParameters() const
{
	int count = 0;
	for (int i = 0; i < InputParameters.Num(); i++)
	{
		if (InputParameters[i].Type != UEdGraphSchema_K2::PC_Wildcard)
		{
			count ++;
		}
	}
	return count;
}

// how many floats does this collection of parameters equate to?
//
int UInteractMLParameterNode::CountFloats() const
{
	int count = 0;
	for (int i = 0; i < InputParameters.Num(); i++)
	{
		count += GetInputParameterInfo( InputParameters[i].Type ).NumFloats;
	}
	return count;
}


// put node in blueprint menu
//
void UInteractMLParameterNode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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
void UInteractMLParameterNode::AllocateDefaultPins()
{
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	//---- Inputs ----

	// Target actor (needed for context)
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, AActor::StaticClass(), FInteractMLParameterNodePinNames::TargetPinName);
	
	//TEMP: testing
	if (InputParameters.Num() == 0)
	{
		InputParameters.Add(FParameterSpec(UEdGraphSchema_K2::PC_Float, "A Float"));
		InputParameters.Add(FParameterSpec(UEdGraphSchema_K2::PC_Int, "An Integer"));
	}

	// Add known input pins
	for(int i = 0; i < InputParameters.Num(); i++)
	{
		CreatePin(EGPD_Input, InputParameters[i].Type, *InputParameters[i].Name);
	}

	//---- Outputs ----

	// Resulting Parameter (ptr) struct
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParametersPtr>::Get(), FInteractMLParameterNodePinNames::OutputPinName);
	
	Super::AllocateDefaultPins();
}

// we've been asked to add a new pin
//
void UInteractMLParameterNode::AddInputPin()
{
	Modify();

	//add pin
	FParameterSpec new_pin_spec( UEdGraphSchema_K2::PC_Wildcard, TEXT( "New Parameter" ) );
	InputParameters.Add( new_pin_spec );
	CreatePin( EGPD_Input, new_pin_spec.Type, *new_pin_spec.Name );

	//update to reflect new pin
	const bool bIsCompiling = GetBlueprint()->bBeingCompiled;
	if( !bIsCompiling )
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}


#if 0
// pin access helpers
//
UEdGraphPin* UInteractMLParameterNode::GetOutputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteracMLParameterNodePinNames::OutputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}
#endif

// runtime node operation functionality hookup
//
void UInteractMLParameterNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );

	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : parameter inputs
//	TMap<int,UEdGraphPin*> param_pins;
//	CollectParameterPins( param_pins );
	//output pins : then (execution continues)
	UEdGraphPin* MainThenPin = FindPin(UEdGraphSchema_K2::PN_Then);	
	//output pins : output (parameter list struct)
//	UEdGraphPin* MainOutputPin = GetOutputPin();

	MainThenPin->MakeLinkTo( MainExecPin );

	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();

#if 0
	//internal parameter source
	UFunction* ParamCreateFunction = FindParameterCreateFunction();
	UK2Node_CallFunction* CallCreateFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallCreateFn->SetFromFunction(ParamCreateFunction);
	CallCreateFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation(CallCreateFn, this);
	//setter fn pins
	UEdGraphPin* CreateFnExecPin = CallCreateFn->GetExecPin();
	UEdGraphPin* CreateFnThenPin = CallCreateFn->GetThenPin();
	UEdGraphPin* CreateFnResultPin = CallCreateFn->GetReturnValuePin();
	//hook up create fn to exec first
	CompilerContext.MovePinLinksToIntermediate( *MainExecPin, *CreateFnExecPin );
	
	//each parameter pin needs a function call node chained together to fully set up the parameter input of the entity
	UEdGraphPin* CurrentThenPin = CreateFnThenPin;
	for(TMap<int,UEdGraphPin*>::TIterator It(param_pins); It; ++It)	
	{
		bool first_time = CurrentThenPin==nullptr;

		//id of param we are hooking up
		Apparance::ValueID ParameterID = (Apparance::ValueID)It.Key();
		UEdGraphPin* ParameterPin = It.Value();	

		//ensure parameter type is correct
		Apparance::Parameter::Type ParameterSpecType = spec->Inputs->FindType( ParameterID );
		Apparance::Parameter::Type ParameterPinType = ApparanceTypeFromPinType( ParameterPin->PinType );
		if(ParameterPinType!=ParameterSpecType)
		{
			CompilerContext.MessageLog.Error( *FString::Format( *LOCTEXT("ParamTypeMismatch", "Type of input parameter {0} has changed to {1}, expected {2}.").ToString(), { (int)ParameterID, ParameterPinType, ParameterSpecType } ), this );
			return;
		}

		//find a function to handle setting this parameter on the entity
		UFunction* ParamSetFunction = FindParameterSetterFunctionByType( ParameterPinType );	
		if (!ParamSetFunction)
		{
			CompilerContext.MessageLog.Error( *FString::Format( *LOCTEXT("MissingParametersParamSetter", "Failed to find function to set parameter list parameter of type {0}.").ToString(), { ParameterPinType } ), this );
			return;
		}

		//create intermediate node to call the setter
		UK2Node_CallFunction* CallSetterFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallSetterFn->SetFromFunction(ParamSetFunction);
		CallSetterFn->AllocateDefaultPins();
		CompilerContext.MessageLog.NotifyIntermediateObjectCreation(CallSetterFn, this);

		//setter fn pins
		UEdGraphPin* FnExecPin = CallSetterFn->GetExecPin();
		UEdGraphPin* FnThenPin = CallSetterFn->GetThenPin();
		UEdGraphPin* FnInputPin = CallSetterFn->FindPinChecked( FApparanceParamsParameterSetterPinNames::InputPinName );
		UEdGraphPin* FnParamPin = CallSetterFn->FindPinChecked( FApparanceParamsParameterSetterPinNames::ParameterIDPinName );
		UEdGraphPin* FnValuePin = CallSetterFn->FindPinChecked( FApparanceParamsParameterSetterPinNames::ValuePinName );
		
		//hook up function inputs to previous outputs
		CurrentThenPin->MakeLinkTo( FnExecPin );

		//hook up parameter collection struct input (same for all setters)
		CreateFnResultPin->MakeLinkTo( FnInputPin );

		//hook up parameter input
		CompilerContext.MovePinLinksToIntermediate( *ParameterPin, *FnValuePin );

		//set param id function input
		FnParamPin->DefaultValue = FString::FromInt( ParameterID );

		//move on to function outputs
		CurrentThenPin = FnThenPin;		
	}

	//hook up last in function node chain to then pin
	CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *CurrentThenPin );

	//hook up parameter collection output
	CompilerContext.MovePinLinksToIntermediate( *MainOutputPin, *CreateFnResultPin );
#endif
}

#undef LOCTEXT_NAMESPACE
