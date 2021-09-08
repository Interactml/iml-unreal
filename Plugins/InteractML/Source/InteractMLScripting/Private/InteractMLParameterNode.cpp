//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLParameterNode.h"

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
#include "InteractMLParameters.h"
#include "InteractMLBlueprintLibrary.h"
#include "InteractMLConstants.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

static FColor cInteractMLPrimaryColour(128, 100, 255); //IML Purple); // 58, 59, 91);

// LOCAL CLASSES & TYPES


//////////////////////////////// PARAMETER INPUT INFO ////////////////////////////////////

// info about types of compatible input parameters
//
struct FInputParameterInfo
{
	FText DisplayName;
	FName Type;
	UScriptStruct* Struct;
	int NumFloats;
	FName ParamAddFunctionName;

	FInputParameterInfo(FText name, FName type, FName add_fn, UScriptStruct* struct_type, int num_floats)
		: DisplayName(name)
		, Type(type)
		, Struct(struct_type)
		, NumFloats(num_floats)
		, ParamAddFunctionName(add_fn)
	{}
	static FInputParameterInfo None;
};

// add accepted input parameter types here
//
static TArray<FInputParameterInfo>& GetInputParameterInfoList()
{
	static TArray<FInputParameterInfo> list;
	if(list.Num()==0)
	{
		list.Add( FInputParameterInfo( LOCTEXT("ParameterNodeTypeNameInt","Integer"),        UEdGraphSchema_K2::PC_Int,     GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, AddIntegerParameter),    nullptr,                             1));
		list.Add( FInputParameterInfo( LOCTEXT("ParameterNodeTypeNameFloat","Float"),        UEdGraphSchema_K2::PC_Float,   GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, AddFloatParameter),      nullptr,                             1));
		list.Add( FInputParameterInfo( LOCTEXT("ParameterNodeTypeNameBool","Boolean"),       UEdGraphSchema_K2::PC_Boolean, GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, AddBooleanParameter),    nullptr,                             1));
		list.Add( FInputParameterInfo( LOCTEXT("ParameterNodeTypeNameVector2D","2D Vector"), UEdGraphSchema_K2::PC_Struct,  GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, AddVector2Parameter),    TBaseStructure<FVector2D>::Get(),    2));
		list.Add( FInputParameterInfo( LOCTEXT("ParameterNodeTypeNameVector","3D Vector"),   UEdGraphSchema_K2::PC_Struct,  GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, AddVector3Parameter),    TBaseStructure<FVector>::Get(),      3));
		list.Add( FInputParameterInfo( LOCTEXT("ParameterNodeTypeNameQuat","Rotation"),      UEdGraphSchema_K2::PC_Struct,  GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, AddQuaternionParameter), TBaseStructure<FQuat>::Get(),        4));
		list.Add( FInputParameterInfo( LOCTEXT("ParameterNodeTypeNameColor","Colour"),       UEdGraphSchema_K2::PC_Struct,  GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, AddColourParameter),     TBaseStructure<FLinearColor>::Get(), 3));
	};
	return list;
}
FInputParameterInfo FInputParameterInfo::None( FText::FromString(TEXT("Unknown")), UEdGraphSchema_K2::PC_Wildcard, NAME_None, nullptr, 0 );

// is this pin type supported for parameter collection?
//
static bool IsCompatibleInputParameterType(FEdGraphPinType type)
{
	static TArray<FInputParameterInfo>& list = GetInputParameterInfoList();
	for (int i = 0; i < list.Num(); i++)
	{
		if (list[i].Type == type.PinCategory
			&& (list[i].Struct==nullptr || list[i].Struct==type.PinSubCategoryObject ))
		{
			return true;
		}
	}
	return false;
}

// info about a supported input parameter type
//
static FInputParameterInfo& GetInputParameterInfo(FEdGraphPinType type)
{
	static TArray<FInputParameterInfo>& list = GetInputParameterInfoList();
	for (int i = 0; i < list.Num(); i++)
	{
		if (list[i].Type == type.PinCategory
		 && (list[i].Struct==nullptr || list[i].Struct==type.PinSubCategoryObject ))
		{
			return list[i];
		}
	}
	return FInputParameterInfo::None;
}

// display name of an input parameter type
//
static FText GetInputParameterDisplayName(FEdGraphPinType type)
{
	FInputParameterInfo& spec = GetInputParameterInfo(type);
	return spec.DisplayName;
}

// all compatible types as a text string
//
static FString GetInputParameterTypesDescription()
{
	static FString types;
	//init on demand
	if(types.Len() == 0)
	{
		TArray<FInputParameterInfo>& list = GetInputParameterInfoList();
		//collect unique type names
		TSet<FString> type_set;
		for(int i = 0; i < list.Num(); i++)
		{
			type_set.Add( list[i].DisplayName.ToString(), nullptr );
		}
		//turn into list
		for(auto type : type_set)
		{
			if(!types.IsEmpty())
			{
				types.Append( ", " );
			}
			types.Append( type );
		}
	}
	return types;
}


// pin and function name constants
//
namespace FInteractMLParameterNodePinNames
{
	//in
	//out
	static const FName ParametersOutputPinName("Parameters");
}  	
namespace FInteractMLParameterUtilityFunctionNames
{
	static const FName AccessParametersName(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, GetParameters));
}
//UInteractMLBlueprintLibrary::GetParameters(...)
namespace FParameterNodeParametersAccessPinNames
{
	static const FName ActorPinName("Actor");
	static const FName GuidPinName("NodeID");
}
//UInteractMLBlueprintLibrary::Add*Parameter(...)
namespace FParameterNodeParameterAddPinNames
{
	static const FName ParametersPinName("Parameters");
	static const FName ValuePinName("Value");
}


/////////////////////////////////// HELPERS /////////////////////////////////////

// extract the ID for a pin encoded in it's name
//
static int ParsePinCode(FName name)
{
	FString n = name.ToString();
	int index = 0;
	if (n.FindChar('#', index))
	{
		int id_index = index + 1;
		int id = FCString::Atoi( &n[id_index] );
		return id;
	}
	//not found
	return 0;
}

// name a pin name the encodes an identifier
//
static FName MakePinCode(int identifier)
{
	return FName(*FString::Format(TEXT("Parameter#{0}"), { identifier }));
}


//////////////////////////////// PARAMETER SPEC STRUCT ////////////////////////////////

// apply a new type to a pin and update spec
//
void FParameterSpec::ApplyPinType(UEdGraphPin* pin, FEdGraphPinType type)
{
	Type = type;
	pin->PinType = type;
}

// apply a new display name to a pin and update spec
//
void FParameterSpec::ApplyPinDisplayName(UEdGraphPin* pin)
{
	if (Name.Len() > 0)
	{
		pin->PinFriendlyName = FText::FromString(Name);
	}
	else
	{
		pin->PinFriendlyName = LOCTEXT("ParameterNodeNewPin", "New Parameter");
	}
}
void FParameterSpec::ApplyPinDisplayName(UEdGraphPin* pin, FText display_name)
{
	Name = display_name.ToString();
	ApplyPinDisplayName( pin );
}

// set a suitable tooltip for an input pin
//
void FParameterSpec::ApplyPinTooltip( UEdGraphPin* pin ) const
{
	FText tt;
	if (Type.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
	{
		//wild
		FString types_list_desc = GetInputParameterTypesDescription();
		tt = FText::Format(LOCTEXT("ParameterNodeInputUnassignedTooltip", "Connect a new parameter here to add it to the collection. Compatible types:\n{0}"), { FText::FromString( types_list_desc ) } );
	}
	else
	{
		//non-connection warning
		FText warning;
		if (pin->LinkedTo.Num() == 0)
		{
			warning = LOCTEXT("ParameterNodeInputConnectWarning", "\nWARNING: Unconnected parameters are invariant and won't contribute anything useful to model training");
		}

		//assigned type
		auto& param_type_info = GetInputParameterInfo(pin->PinType);
		FText format;
		if (param_type_info.NumFloats == 1) //singular vs plural
		{
			format = LOCTEXT("ParameterNodeInputAssignedTooltip", "{0} input parameter, contributing {1} distinct value to the collection{2}");
		}
		else
		{
			format = LOCTEXT("ParameterNodeInputAssignedTooltip", "{0} input parameter, contributing {1} distinct values to the collection{2}");
		}
		tt = FText::Format(format, {param_type_info.DisplayName, param_type_info.NumFloats, warning});
	}
	pin->PinToolTip = tt.ToString();
}


//////////////////////////////// PARAMETER NODE CLASS ////////////////////////////////////

// node title
//
FText UInteractMLParameterNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText node_name = LOCTEXT("ParameterNodeTitle", "Collect All The Things!");

	//check what's needed
	switch (TitleType)
	{
		case ENodeTitleType::FullTitle:
		{
			FString title = node_name.ToString();
			int param_count = CountParameters();
			int float_count = CountFloats();
			title.Append(TEXT("\n"));
			if(param_count > 0)
			{
				title.Append( FText::Format( LOCTEXT( "ParameterNodeSubTitleCount", "{0} parameter(s) so far ({1} values)" ), param_count, float_count ).ToString() );
			}
			else
			{
				title.Append( FText::Format( LOCTEXT( "ParameterNodeSubTitleDesc", "Build a parameter set to record/recognise" ), param_count, float_count ).ToString() );
			}
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

// node tooltip
//
FText UInteractMLParameterNode::GetTooltipText() const
{
	return LOCTEXT("ParameterNodeTooltip", "Connect all the parameters to be used as training inputs");
}

// how many valid parameters does this collection have?
//
int UInteractMLParameterNode::CountParameters() const
{
	int count = 0;
	for (int i = 0; i < InputParameters.Num(); i++)
	{
		if (InputParameters[i].Type.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
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


// context menu that appears on the node
//
void UInteractMLParameterNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection("InteractMLParameterNode", LOCTEXT("ParameterNodeContextMenuSection", "Parameter Inputs"));
		if (Context->Pin != NULL)
		{
			const FParameterSpec* pin_spec = FindPinSpec(Context->Pin);
			if(pin_spec)
			{
				Section.AddMenuEntry(
					"RemovePin",
					LOCTEXT("ParameterNodeRemovePin", "Remove parameter"),
					LOCTEXT("ParameterNodeRemovePinTooltip", "Remove this parameter pin from the collection"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateUObject( const_cast<UInteractMLParameterNode*>(this), &UInteractMLParameterNode::RemoveParameterInput, const_cast<UEdGraphPin*>( Context->Pin ))
					)
				);
			}
		}
	}
}

// block connection of unsupported pin types
// true if pins cannot be connected due to node's inner logic, put message for user in OutReason
//
bool UInteractMLParameterNode::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	//only want to validate unassigned parameter inputs
	UEdGraphPin* exec_pin = GetExecPin();
	UEdGraphPin* actor_pin = GetActorInputPin();
	if(MyPin->Direction != EGPD_Input || MyPin->PinType.PinCategory!=UEdGraphSchema_K2::PC_Wildcard || MyPin == exec_pin || MyPin == actor_pin)
	{
		//allowed
		return false;
	}

	//check
	if (!IsCompatibleInputParameterType(OtherPin->PinType))
	{
		//reason
		OutReason = LOCTEXT("ParameterNodeUnsupportedType", "This parameter type is not supported.\nTry converting it or extracting a supported type of value from it.").ToString();

		//not allowed
		return true;
	}

	//allowed
	return false;
}

// we can be notified of rebuild
//
void UInteractMLParameterNode::PostReconstructNode()
{
	Super::PostReconstructNode();

	//re-apply tooltips after load/AllocateDefaultPins since they need to re-gen once actually connected up again (not until after AllocateDefaultPins)
	for(auto pin : Pins)
	{
		FParameterSpec* pin_spec = FindPinSpec( pin );
		if(pin_spec)
		{
			pin_spec->ApplyPinTooltip( pin );
		}
	}
}

// custom pins
//
void UInteractMLParameterNode::AllocateDefaultPins()
{
	//handle actor context pin
	Super::AllocateDefaultPins();

	//---- Inputs ----
	
	// Add known input pins
	for(int i = 0; i < InputParameters.Num(); i++)
	{
		//create
		FName code_name = MakePinCode( InputParameters[i].Identifier );
		UEdGraphPin* pin = CreatePin(EGPD_Input, InputParameters[i].Type, code_name );
		
		//setup
		InputParameters[i].ApplyPinDisplayName( pin );
		InputParameters[i].ApplyPinTooltip( pin );
	}

	//---- Outputs ----

	// Resulting Parameter (ptr) struct
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParameters>::Get(), FInteractMLParameterNodePinNames::ParametersOutputPinName);
	
}

// we've been asked to add a new pin
//
void UInteractMLParameterNode::AddInputPin()
{
	//this is an undoable edit
	Modify();

	//generate ID for pin
	int new_id = 1;
	for (int i = 0; i < InputParameters.Num(); i++)
	{
		//ensure above any existing ones
		new_id = FMath::Max(new_id, InputParameters[i].Identifier + 1);
	}

	//add pin record
	FEdGraphPinType wildcard_type( UEdGraphSchema_K2::PC_Wildcard, NAME_None, nullptr, EPinContainerType::None, /*bIsReference =*/ false, /*InValueTerminalType=*/FEdGraphTerminalType() );
	FParameterSpec new_pin_spec( wildcard_type, new_id );
	InputParameters.Add( new_pin_spec );

	//create pin
	FName code_name = MakePinCode( new_id );
	UEdGraphPin* new_pin = CreatePin( EGPD_Input, new_pin_spec.Type, code_name );
	new_pin_spec.ApplyPinDisplayName( new_pin );
	new_pin_spec.ApplyPinTooltip( new_pin );
	
	//update to reflect new pin
	const bool bIsCompiling = GetBlueprint()->bBeingCompiled;
	if( !bIsCompiling )
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

// we've been asked to remove an existing pin
//
void UInteractMLParameterNode::RemoveParameterInput( UEdGraphPin* pin )
{
	//locate and remove from list
	FParameterSpec* pin_spec = FindPinSpec(pin);
	if(pin_spec)
	{
		const FScopedTransaction Transaction( LOCTEXT( "ParameterNodeRemoveInput", "Removing parameter input pin" ) );

		//this is an undoable edit
		GetGraph()->Modify();	//connections are potentially affected by deleting a pin
		Modify();

		//remove spec
		for (int i = 0; i < InputParameters.Num(); i++)
		{
			if (InputParameters[i].Identifier == pin_spec->Identifier)
			{
				InputParameters.RemoveAt(i);
				break;
			}
		}

		//node change
		RemovePin(pin);

		//update to reflect new pin
		const bool bIsCompiling = GetBlueprint()->bBeingCompiled;
		if(!bIsCompiling)
		{
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified( GetBlueprint() );
		}
	}
}

// pin access helpers : parameters output
//
UEdGraphPin* UInteractMLParameterNode::GetParametersOutputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLParameterNodePinNames::ParametersOutputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

// locate the spec info for a given pin
// NOTE: temp ptr, don't keep
//
FParameterSpec* UInteractMLParameterNode::FindPinSpec(const UEdGraphPin* pin)
{
	int pin_code = ParsePinCode( pin->PinName );
	if(pin_code != 0)
	{
		for(int i = 0; i < InputParameters.Num(); i++)
		{
			if(InputParameters[i].Identifier == pin_code)
			{
				return &InputParameters[i];
			}
		}
	}
	return nullptr;
}

// const version of above
//
const FParameterSpec* UInteractMLParameterNode::FindPinSpec(const UEdGraphPin* pin) const
{
	int pin_code = ParsePinCode( pin->PinName );
	for (int i = 0; i < InputParameters.Num(); i++)
	{
		if (InputParameters[i].Identifier == pin_code)
		{
			return &InputParameters[i];
		}
	}
	
	return nullptr;
}

// handle new connections
//
void UInteractMLParameterNode::NotifyPinConnectionListChanged(UEdGraphPin* pin)
{
	Super::NotifyPinConnectionListChanged( pin );

	//find spec
	FParameterSpec* pin_spec = FindPinSpec( pin );
	if(!pin_spec)
	{
		//ignore non-parameter input pins
		return;
	}

	//become connected?
	UEdGraphPin* connected_to = (pin->LinkedTo.Num() > 0)?pin->LinkedTo[0]:nullptr;
	if (connected_to)
	{
		//is wild? i.e. awaiting type assignment
		if (pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
		{
			//adopt type
			pin_spec->ApplyPinType(pin, connected_to->PinType);

			//name after type
			FText name = GetInputParameterDisplayName(connected_to->PinType);
			pin_spec->ApplyPinDisplayName(pin, name);
		}
	}

	//always update tooltip (disconnected pins come with a warning)
	pin_spec->ApplyPinTooltip( pin );
}

// runtime node operation functionality hookup
//
void UInteractMLParameterNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph )
{
	Super::ExpandNode( CompilerContext, SourceGraph );

	//input pins : exec (execution triggered)
	UEdGraphPin* MainExecPin = GetExecPin();
	//input pins : parameter inputs
	TMap<UEdGraphPin*,FParameterSpec*> param_pins;
	CollectParameterPins( param_pins );
	//output pins : then (execution continues)
	UEdGraphPin* MainThenPin = FindPin( UEdGraphSchema_K2::PN_Then );	
	//output pins : output (parameter struct)
	UEdGraphPin* MainOutputPin = GetParametersOutputPin();

	//internal parameter collection source
	UFunction* ParamAccessFn = FindParameterAccessFunction();
	UK2Node_CallFunction* CallParamAccessFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallParamAccessFn->SetFromFunction( ParamAccessFn );
	CallParamAccessFn->AllocateDefaultPins();
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation( CallParamAccessFn, this);
	//setter fn pins
	UEdGraphPin* ParamFnExecPin = CallParamAccessFn->GetExecPin();
	UEdGraphPin* ParamFnThenPin = CallParamAccessFn->GetThenPin();
	UEdGraphPin* ParamFnResultPin = CallParamAccessFn->GetReturnValuePin();
	UEdGraphPin* ParamFnActorPin = CallParamAccessFn->FindPinChecked( FParameterNodeParametersAccessPinNames::ActorPinName );
	UEdGraphPin* ParamFnGuidPin = CallParamAccessFn->FindPinChecked( FParameterNodeParametersAccessPinNames::GuidPinName );
	//hook up create fn to exec first
	CompilerContext.MovePinLinksToIntermediate( *MainExecPin, *ParamFnExecPin );
	//hook up other data parameter access needs
	ConnectContextActor(CompilerContext, SourceGraph, ParamFnActorPin);
	ParamFnGuidPin->DefaultValue = NodeGuid.ToString( EGuidFormats::Digits );	//node disambiguation/context
	
	//each parameter input pin needs a function call node chained together to fully set up the parameter collection
	UEdGraphPin* CurrentThenPin = ParamFnThenPin;
	for(TMap<UEdGraphPin*,FParameterSpec*>::TIterator It(param_pins); It; ++It)	
	{
		UEdGraphPin* ParameterPin = It.Key();
		FParameterSpec* param_spec = It.Value();

		//find a function to handle addung this parameter to the collection
		UFunction* ParamAddFn = FindParameterAddFunctionByType( &ParameterPin->PinType );	
		if (!ParamAddFn)
		{
			CompilerContext.MessageLog.Error( *FText::Format( LOCTEXT("ParameterNodeMissingParamSetter", "Failed to find function to add parameter collection parameter of type '{0}'."), FText::FromName( ParameterPin->PinType.PinCategory ) ).ToString(), this );
			return;
		}

		//create intermediate node to call the setter
		UK2Node_CallFunction* CallParamAddFn = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallParamAddFn->SetFromFunction( ParamAddFn );
		CallParamAddFn->AllocateDefaultPins();
		CompilerContext.MessageLog.NotifyIntermediateObjectCreation(CallParamAddFn, this);

		//setter fn pins
		UEdGraphPin* FnExecPin = CallParamAddFn->GetExecPin();
		UEdGraphPin* FnThenPin = CallParamAddFn->GetThenPin();
		UEdGraphPin* FnParamsPin = CallParamAddFn->FindPinChecked( FParameterNodeParameterAddPinNames::ParametersPinName );
		UEdGraphPin* FnValuePin = CallParamAddFn->FindPinChecked( FParameterNodeParameterAddPinNames::ValuePinName );
		
		//hook up function call chain from prev to call this
		CurrentThenPin->MakeLinkTo( FnExecPin );
		CurrentThenPin = FnThenPin; //this is now the next exe pin
		
		//hook up parameter collection struct input (same for all setters)
		ParamFnResultPin->MakeLinkTo( FnParamsPin );

		//hook up parameter value input
		CompilerContext.MovePinLinksToIntermediate( *ParameterPin, *FnValuePin );
	}

	//hook up last in function node chain to then pin
	CompilerContext.MovePinLinksToIntermediate( *MainThenPin, *CurrentThenPin );

	//hook up parameter collection output
	CompilerContext.MovePinLinksToIntermediate( *MainOutputPin, *ParamFnResultPin );

	//After we are done we break all links to this node (not the internally created one)
	//leaving the newly created internal nodes left to do the work
	BreakAllNodeLinks();
}

// collect together the value input pins, associated with the param spec they correspond to
//
void UInteractMLParameterNode::CollectParameterPins(TMap<UEdGraphPin*, FParameterSpec*>& param_pins)
{
	for(UEdGraphPin* pin : Pins)
	{
		FParameterSpec* pin_spec = FindPinSpec( pin );
		if (pin_spec && pin_spec->Type.PinCategory!=UEdGraphSchema_K2::PC_Wildcard)
		{
			//is an input with a type assigned, record
			param_pins.Add( pin, pin_spec );
		}
	}
}

// locate function used to obtain the parameter collection object
//
UFunction* UInteractMLParameterNode::FindParameterAccessFunction() const
{
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( FInteractMLParameterUtilityFunctionNames::AccessParametersName );
}

// locate function used to set a specific type of parameter on a parameter collection object
//
UFunction* UInteractMLParameterNode::FindParameterAddFunctionByType(FEdGraphPinType* pin_type)
{
	FInputParameterInfo& param_info = GetInputParameterInfo(*pin_type);
	if (param_info.Type == UEdGraphSchema_K2::PC_Wildcard)
	{
		//not a supported type!
		return nullptr;
	}

	//function to use
	FName param_set_fn_name = param_info.ParamAddFunctionName;

	//library lookup of fn
	UClass* LibraryClass = UInteractMLBlueprintLibrary::StaticClass();
	return LibraryClass->FindFunctionByName( param_set_fn_name );	
}


#undef LOCTEXT_NAMESPACE
