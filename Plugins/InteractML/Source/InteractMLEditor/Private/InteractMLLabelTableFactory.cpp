//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

//main
#include "InteractMLLabelTableFactory.h"

//unreal
#include "UserDefinedStructure/UserDefinedStructEditorData.h"
#include "StructViewerFilter.h"
#include "DataTableEditorUtils.h"
#include "StructViewerModule.h"

//module
#include "InteractMLLabel.h"
#include "InteractMLLabelTable.h"

//module editor


#define LOCTEXT_NAMESPACE "InteractML"

////////////////////////////// LABEL TABLE /////////////////////////////

// factory setup
//
UInteractMLLabelTableFactory::UInteractMLLabelTableFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UInteractMLLabelTable::StaticClass();
}

// NOTE: Copy of UDataTableFactory::ConfigureProperties with additional filtering to restrict to label types
//
bool UInteractMLLabelTableFactory::ConfigureProperties()
{
	class FDataTableStructFilter : public IStructViewerFilter
	{
		TSet<const UScriptStruct*> DeDuplicate;
	public:
		virtual bool IsStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const UScriptStruct* InStruct, TSharedRef<FStructViewerFilterFuncs> InFilterFuncs) override
		{
			bool valid = FDataTableEditorUtils::IsValidTableStruct(InStruct);
			if (valid)
			{
				//is this a label type?
				valid = InStruct->IsA(UInteractMLLabel::StaticClass());
				
				//for some reason our label types appear twice here :/
				if(valid)
				{
					valid = !DeDuplicate.Contains(InStruct);
					if (valid)
					{
						DeDuplicate.Add(InStruct);
					}
				}
			}
			return valid;
		}
		
		virtual bool IsUnloadedStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const FName InStructPath, TSharedRef<FStructViewerFilterFuncs> InFilterFuncs) override
		{
			// Unloaded structs are always User Defined Structs, and User Defined Structs are always allowed
			// They will be re-validated by IsStructAllowed once loaded during the pick
			
			//we seem to need to force the issue with exact type match as unloaded custom types still appear in the list
			return InStructPath.ToString().EndsWith( TEXT(".InteractMLLabel") );
		}
	};
	
	class FDataTableFactoryUI : public TSharedFromThis<FDataTableFactoryUI>
	{
	public:
		FReply OnCreate()
		{
			check(ResultStruct);
			if (PickerWindow.IsValid())
			{
				PickerWindow->RequestDestroyWindow();
			}
			return FReply::Handled();
		}
		
		FReply OnCancel()
		{
			ResultStruct = nullptr;
			if (PickerWindow.IsValid())
			{
				PickerWindow->RequestDestroyWindow();
			}
			return FReply::Handled();
		}
		
		bool IsStructSelected() const
		{
			return ResultStruct != nullptr;
		}
		
		void OnPickedStruct(const UScriptStruct* ChosenStruct)
		{
			ResultStruct = ChosenStruct;
			StructPickerAnchor->SetIsOpen(false);
		}
		
		FText OnGetComboTextValue() const
		{
			return ResultStruct
				? FText::AsCultureInvariant(ResultStruct->GetName())
			: LOCTEXT("None", "None");
		}
		
		TSharedRef<SWidget> GenerateStructPicker()
		{
			FStructViewerModule& StructViewerModule = FModuleManager::LoadModuleChecked<FStructViewerModule>("StructViewer");
			
			// Fill in options
			FStructViewerInitializationOptions Options;
			Options.Mode = EStructViewerMode::StructPicker;
			Options.StructFilter = MakeShared<FDataTableStructFilter>();
			Options.DisplayMode = EStructViewerDisplayMode::ListView;
			Options.NameTypeToDisplay = EStructViewerNameTypeToDisplay::DisplayName;
			Options.bAllowViewOptions = false;
			
			return
				SNew(SBox)
				.WidthOverride(330)
				[
				SNew(SVerticalBox)
				
				+SVerticalBox::Slot()
				.FillHeight(1.0f)
				.MaxHeight(500)
				[
				SNew(SBorder)
				.Padding(4)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				[
				StructViewerModule.CreateStructViewer(Options, FOnStructPicked::CreateSP(this, &FDataTableFactoryUI::OnPickedStruct))
				]
				]
				];
		}
		
		const UScriptStruct* OpenStructSelector()
		{
			FStructViewerModule& StructViewerModule = FModuleManager::LoadModuleChecked<FStructViewerModule>("StructViewer");
			ResultStruct = nullptr;
			
			// Fill in options
			FStructViewerInitializationOptions Options;
			Options.Mode = EStructViewerMode::StructPicker;
			Options.StructFilter = MakeShared<FDataTableStructFilter>();
			
			PickerWindow = SNew(SWindow)
				.Title(LOCTEXT("LabelTableFactoryOptions", "Pick Label Type"))
				.ClientSize(FVector2D(350, 100))
				.SupportsMinimize(false)
				.SupportsMaximize(false)
				[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
				.Padding(10)
				[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
				SAssignNew(StructPickerAnchor, SComboButton)
				.ContentPadding(FMargin(2,2,2,1))
				.MenuPlacement(MenuPlacement_BelowAnchor)
				.ButtonContent()
				[
				SNew(STextBlock)
				.Text(this, &FDataTableFactoryUI::OnGetComboTextValue)
				]
				.OnGetMenuContent(this, &FDataTableFactoryUI::GenerateStructPicker)
				]
				+SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
				SNew(SButton)
				.Text(LOCTEXT("OK", "OK"))
				.IsEnabled(this, &FDataTableFactoryUI::IsStructSelected)
				.OnClicked(this, &FDataTableFactoryUI::OnCreate)
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
				SNew(SButton)
				.Text(LOCTEXT("Cancel", "Cancel"))
				.OnClicked(this, &FDataTableFactoryUI::OnCancel)
				]
				]
				]
				];
			
			GEditor->EditorAddModalWindow(PickerWindow.ToSharedRef());
			PickerWindow.Reset();
			
			return ResultStruct;
		}
		
	private:
		TSharedPtr<SWindow> PickerWindow;
		TSharedPtr<SComboButton> StructPickerAnchor;
		const UScriptStruct* ResultStruct = nullptr;
	};
	
	TSharedRef<FDataTableFactoryUI> StructSelector = MakeShareable(new FDataTableFactoryUI());
	Struct = StructSelector->OpenStructSelector();
	
	return Struct != nullptr;
}

UDataTable* UInteractMLLabelTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	return NewObject<UInteractMLLabelTable>(InParent, Name, Flags);
}

#undef LOCTEXT_NAMESPACE

