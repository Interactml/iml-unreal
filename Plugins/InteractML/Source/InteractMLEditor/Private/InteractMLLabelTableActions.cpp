//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

//main
#include "InteractMLLabelTableActions.h"

//unreal
#include "BlueprintEditorModule.h"
#include "Editor/DataTableEditor/Public/DataTableEditorModule.h"

//module
#include "InteractMLLabelTable.h"

//module editor


#define LOCTEXT_NAMESPACE "InteractML"


////////////////////////////// LABEL TABLE /////////////////////////////

// IAssetTypeActions Implementation

// display name of asset type
//
FText FInteractMLLabelTableActions::GetName() const
{
	return LOCTEXT("LabelTableAssetName", "Label Table");
}

// 'colour' of asset (used on various UI flashes to help visual identification)
//
FColor FInteractMLLabelTableActions::GetTypeColor() const
{
	return FColor::FromHex("888EF7"); //InteractML Bright Purple
}

// actual asset class we represent
//
UClass* FInteractMLLabelTableActions::GetSupportedClass() const
{
	return UInteractMLLabelTable::StaticClass();
}

// asset categorisation (comes from IML editor module startup)
//
uint32 FInteractMLLabelTableActions::GetCategories()
{
	return RegisteredCategoryHandle;
}

// open an explicit editor for this asset instead of the default details panel
// NOTE: copied from FAssetTypeActions_Struct::OpenAssetEditor as we want the same functionality but we can't derive (private)
//
void FInteractMLLabelTableActions::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor )
{
	TArray<UDataTable*> DataTablesToOpen;
	TArray<UDataTable*> InvalidDataTables;
	
	for (UObject* Obj : InObjects)
	{
		UDataTable* Table = Cast<UDataTable>(Obj);
		if (Table)
		{
			if (Table->GetRowStruct())
			{
				DataTablesToOpen.Add(Table);
			}
			else
			{
				InvalidDataTables.Add(Table);
			}
		}
	}
	
	if (InvalidDataTables.Num() > 0)
	{
		FTextBuilder DataTablesListText;
		DataTablesListText.Indent();
		for (UDataTable* Table : InvalidDataTables)
		{
			const FName ResolvedRowStructName = Table->GetRowStructName();
			DataTablesListText.AppendLineFormat(LOCTEXT("DataTable_MissingRowStructListEntry", "* {0} (Row Structure: {1})"), FText::FromString(Table->GetName()), FText::FromName(ResolvedRowStructName));
		}
		
		FText Title = LOCTEXT("DataTable_MissingRowStructTitle", "Continue?");
		const EAppReturnType::Type DlgResult = FMessageDialog::Open(
			EAppMsgType::YesNoCancel, 
			FText::Format(LOCTEXT("DataTable_MissingRowStructMsg", "The following Data Tables are missing their row structure and will not be editable.\n\n{0}\n\nDo you want to open these data tables?"), DataTablesListText.ToText()), 
			&Title
		);
		
		switch(DlgResult)
		{
			case EAppReturnType::Yes:
				DataTablesToOpen.Append(InvalidDataTables);
				break;
			case EAppReturnType::Cancel:
				return;
			default:
				break;
		}
	}
	
	FDataTableEditorModule& DataTableEditorModule = FModuleManager::LoadModuleChecked<FDataTableEditorModule>("DataTableEditor");
	for (UDataTable* Table : DataTablesToOpen)
	{
		DataTableEditorModule.CreateDataTableEditor(EToolkitMode::Standalone, EditWithinLevelEditor, Table);
	}
}


#undef LOCTEXT_NAMESPACE

