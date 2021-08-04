//----
// InteractML - University Of Arts London
//----

//main
#include "InteractMLLabelFactory.h"

//unreal
#include "UserDefinedStructure/UserDefinedStructEditorData.h"

//module
#include "InteractMLLabel.h"

//module editor


#define LOCTEXT_NAMESPACE "InteractML"

////////////////////////////// LABEL /////////////////////////////

UInteractMLLabelFactory::UInteractMLLabelFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UInteractMLLabel::StaticClass();
}

UObject* UInteractMLLabelFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//copied from: FStructureEditorUtils::CreateUserDefinedStruct
	
	UInteractMLLabel* Struct = NewObject<UInteractMLLabel>(InParent, Name, Flags | RF_Transactional);
	check(Struct);
	Struct->EditorData = NewObject<UUserDefinedStructEditorData>(Struct, NAME_None, RF_Transactional);
	check(Struct->EditorData);
	
	Struct->Guid = FGuid::NewGuid();
	Struct->SetMetaData(TEXT("BlueprintType"), TEXT("true"));
	Struct->Bind();
	Struct->StaticLink(true);
	Struct->Status = UDSS_Error;
	
	{
		FStructureEditorUtils::AddVariable(Struct, FEdGraphPinType(UEdGraphSchema_K2::PC_Boolean, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType()));
	}
	
	return Struct;
}

#undef LOCTEXT_NAMESPACE

