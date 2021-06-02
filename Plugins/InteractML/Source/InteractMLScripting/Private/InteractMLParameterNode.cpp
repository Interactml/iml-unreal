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
#include "ToolMenu.h" //UToolMenu
#include "ScopedTransaction.h" //FScopedTransaction

//module
#include "InteractMLParameters.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

static FColor cInteractMLPrimaryColour(128, 100, 255); //IML Purple); // 58, 59, 91);

// LOCAL CLASSES & TYPES


// info about types of compatible input parameters
//
struct FInputParameterInfo
{
	FText DisplayName;
	FName Type;
	UScriptStruct* Struct;
	int NumFloats;

	FInputParameterInfo(FText name, FName type, int num_floats)
		: DisplayName(name)
		, Type(type)
		, Struct(nullptr)
		, NumFloats(num_floats)
	{}
	FInputParameterInfo(FText name, FName type, UScriptStruct* struct_type, int num_floats)
		: DisplayName(name)
		, Type(type)
		, Struct(struct_type)
		, NumFloats(num_floats)
	{}
};
static TArray<FInputParameterInfo>& GetInputParameterInfoList()
{
	static TArray<FInputParameterInfo> list;
	if(list.Num()==0)
	{
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameInt","Integer"), UEdGraphSchema_K2::PC_Int, 1));
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameFloat","Float"), UEdGraphSchema_K2::PC_Float, 1));
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameBool","Boolean"), UEdGraphSchema_K2::PC_Boolean, 1));
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameVector2D","2D Vector"), UEdGraphSchema_K2::PC_Struct, TBaseStructure<FVector2D>::Get(), 2));
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameVector","3D Vector"), UEdGraphSchema_K2::PC_Struct, TBaseStructure<FVector>::Get(), 3));
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameQuat","Rotation"), UEdGraphSchema_K2::PC_Struct, TBaseStructure<FQuat>::Get(), 4));
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameColor","Colour"), UEdGraphSchema_K2::PC_Struct, TBaseStructure<FColor>::Get(), 3));
		list.Add(FInputParameterInfo(LOCTEXT("ParameterNodeTypeNameColor","Colour"), UEdGraphSchema_K2::PC_Struct, TBaseStructure<FLinearColor>::Get(), 3));
	};
	return list;
}

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
	static FInputParameterInfo none(FText::FromString(TEXT("Unknown")),UEdGraphSchema_K2::PC_Wildcard, 0);
	return none;
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
	static const FName ActorInputPinName("Actor");
	//out
	static const FName ParametersOutputPinName("Parameters");
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

// basic node properties
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
			title.Append( FText::Format(LOCTEXT("ParameterNodeSubTitle", "{0} parameter(s) so far ({1} values)"), param_count, float_count ).ToString() );
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
	return LOCTEXT("ParameterNodeTooltip", "Connect all the parameters to be used as training inputs");
}
FText UInteractMLParameterNode::GetMenuCategory() const
{
	return LOCTEXT("ParameterNodeMenuCategory", "InteractML");
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
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	//---- Inputs ----

	// Target actor (needed for context)
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, AActor::StaticClass(), FInteractMLParameterNodePinNames::ActorInputPinName);
	
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
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FInteractMLParametersPtr>::Get(), FInteractMLParameterNodePinNames::ParametersOutputPinName);
	
	Super::AllocateDefaultPins();
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

// pin access helpers : actor input
//
UEdGraphPin* UInteractMLParameterNode::GetActorInputPin() const
{
	UEdGraphPin* Pin = FindPin(FInteractMLParameterNodePinNames::ActorInputPinName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

// locate the spec info for a given pin
// NOTE: temp ptr, don't keep
//
FParameterSpec* UInteractMLParameterNode::FindPinSpec(const UEdGraphPin* pin)
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
			CompilerContext.MessageLog.Error( *FString::Format( *LOCTEXT("ParameterNodeTypeMismatch", "Type of input parameter {0} has changed to {1}, expected {2}.").ToString(), { (int)ParameterID, ParameterPinType, ParameterSpecType } ), this );
			return;
		}

		//find a function to handle setting this parameter on the entity
		UFunction* ParamSetFunction = FindParameterSetterFunctionByType( ParameterPinType );	
		if (!ParamSetFunction)
		{
			CompilerContext.MessageLog.Error( *FString::Format( *LOCTEXT("ParameterNodeMissingParamSetter", "Failed to find function to set parameter list parameter of type {0}.").ToString(), { ParameterPinType } ), this );
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
