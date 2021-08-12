//----
// InteractML - University Of Arts London
//----

//main
#include "InteractMLTrainingSetEditor.h"

//unreal
#include "Core.h"
#include "Editor.h"
//unreal:ui
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SScaleBox.h"
#include "WorkflowOrientedApp/SContentReference.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "SAssetDropTarget.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

//module
#include "InteractMLTrainingSet.h"
#include "InteractMLEditor.h"
#include "InteractMLLabel.h"

#define LOCTEXT_NAMESPACE "InteractML"


const FName FTrainingSetEditor::TrainingSetEditorAppIdentifier( TEXT( "InteractMLTrainingSetEditorApp" ) );
const FName FTrainingSetEditor::TrainingSetHierarchyTabId("InteractMLTrainingSetEditor_Hierarchy");
//const FName FTrainingSetEditor::TrainingSetDetailsTabId("InteractMLTrainingSetEditor_Details");




//////////////////////////////////////////////////////////////////////////
// FTrainingSetTreeItem

//ctor
//
FTrainingSetTreeItem::FTrainingSetTreeItem( UInteractMLTrainingSet* pexamples, int example_index, FTrainingSetTreeItem::Ptr parent, int index_in_parent )
	: Examples( pexamples )
	, ExampleIndex( example_index )
	, Parent( parent.Get() )
	, IndexInParent(index_in_parent)
{
	ExampleID = pexamples->GetExamples()[example_index].ID;
}

FString FTrainingSetTreeItem::GetLabel() const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
	if (pexamples)
	{
		const FInteractMLExample& example = pexamples->GetExamples()[ExampleIndex];
		return FString::FromInt( (int)example.label );
	}

	return TEXT("");
}

const FSlateBrush* FTrainingSetTreeItem::GetIcon( bool is_open ) const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
#if 0
	if(pexamples)
	{
		FName icon_name;
		//ask for icon to use
		//icon_name = pexamples->GetIconName( is_open );

		//resolve
		const FSlateBrush* picon = FEditorStyle::GetBrush( icon_name );
		if(picon)
		{
			return picon;
		}
	}
#endif
	return FInteractMLEditorModule::GetStyle()->GetBrush("TreeViewIcons.SingleExample_16x");
}

int FTrainingSetTreeItem::GetSamples() const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
	if (pexamples)
	{
		const FInteractMLExample& example = pexamples->GetExamples()[ExampleIndex];
		return example.inputSeries.Num();
	}
	
	return 0;
}
FText FTrainingSetTreeItem::GetDurationText() const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
	if (pexamples && pexamples->IsSeriesSamples())
	{
		//duration of the series of samples
		const FInteractMLExample& example = pexamples->GetExamples()[ExampleIndex];
		float duration = 0;
		if (example.Duration != 0)
		{
			return FText::Format(LOCTEXT("TrainingSetSeriesDurationFormat", "{0}"), FText::FromString(FString::Printf(TEXT("%.3f"), example.Duration )));
		}
		else
		{
			float approx_duration = (float)example.inputSeries.Num() / 60.0f;	//assume 60FPS to give rough duration
			return FText::Format(LOCTEXT("TrainingSetSeriesDurationApproxFormat", "~{0}"), FText::FromString(FString::Printf(TEXT("%.3f"), approx_duration )));
		}
	}
	
	return FText();
}
FText FTrainingSetTreeItem::GetSessionText() const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
	if (pexamples)
	{
		const FInteractMLExample& example = pexamples->GetExamples()[ExampleIndex];
		return FText::FromString(example.Session);
	}
	return FText();
}
FText FTrainingSetTreeItem::GetUser() const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
	if (pexamples)
	{
		const FInteractMLExample& example = pexamples->GetExamples()[ExampleIndex];
		return FText::FromString(example.User);
	}
	return FText();
}




// update our whole hierarchy to match the provided resource list
//
void FTrainingSetTreeItem::Sync(UInteractMLTrainingSet* proot )
{
	Sync( proot, nullptr, 0 );
}

// update our sub-structure to match the provided resource list entry sub-structure
void FTrainingSetTreeItem::Sync(UInteractMLTrainingSet* pexamples, FTrainingSetTreeItem::Ptr parent, int index_in_parent )
{
	//ensure we are in sync (no need to check+update, just set)
	Examples = pexamples;
	Parent = parent.Get();
	IndexInParent = index_in_parent;

	//source is normal entry hierarchy
#if 0
	int num_children = Examples->Children.Num();
	TArray<UInteractMLTrainingSet*> ordered_resource_entries( Resource->Children );

	//sync children to source
	Children.SetNum( num_children, false );
	for(int i=0 ; i<num_children ; i++)
	{
		if(!Children[i].IsValid())
		{
			Children[i] = MakeShareable( new FTrainingSetTreeItem() );
		}
		Children[i]->Sync( ordered_resource_entries[i], AsShared(), i ); //sub-entries
	}
#endif
}

// sort our child list alphabettically
//
void FTrainingSetTreeItem::Sort()
{
	// lexical order comparison functor
	static struct
	{
		FORCEINLINE bool operator()(const FTrainingSetTreeItem::Ptr A, const FTrainingSetTreeItem::Ptr B) const
		{
			return 0;// A->GetName().Compare(B->GetName()) < 0;
		}
	} lexical_compare;

	// perform lexical sort
	Children.Sort( lexical_compare );
}

// sort a list of entries alphabettically
//
void FTrainingSetTreeItem::Sort( TArray<UInteractMLTrainingSet*>& entries )
{
	// lexical order comparison functor
	static struct
	{
		FORCEINLINE bool operator()( const UInteractMLTrainingSet& A, const UInteractMLTrainingSet& B ) const
		{
			return A.GetName().Compare( B.GetName() ) < 0;
		}
	} lexical_compare;

	// perform lexical sort
	entries.Sort( lexical_compare );
}



//////////////////////////////////////////////////////////////////////////
// FTrainingSetEditor

// raw init, maybe a better place for editor opening
//
FTrainingSetEditor::FTrainingSetEditor()
{
	// hook editor events
	//FInteractMLEditorModule::GetModule()->TrainingSetStructuralChangeEvent.AddRaw( this, &FTrainingSetEditor::OnTrainingSetStructuralChange );
	
	// we want to be updated regularly
	TickDelegate = FTickerDelegate::CreateRaw(this, &FTrainingSetEditor::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);	
}

// raw shutdown, maybe a better place for editor closing
//
FTrainingSetEditor::~FTrainingSetEditor()
{
	//shut down timers
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	TickDelegateHandle.Reset();
	TickDelegate = nullptr;

	//done with events
	//FInteractMLEditorModule::GetModule()->TrainingSetStructuralChangeEvent.RemoveAll( this );
}

// target object access
//
UInteractMLTrainingSet* FTrainingSetEditor::GetEditableExamples() const
{
	return Cast<UInteractMLTrainingSet>( GetEditingObject() );
}


// main setup
//
void FTrainingSetEditor::InitTrainingSetEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UInteractMLTrainingSet* training_set)
{
	//initial layout?
	TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_CompositeTrainingSetEditor_new3_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(TrainingSetHierarchyTabId, ETabState::OpenedTab)
					->SetForegroundTab(TrainingSetHierarchyTabId)
				)
#if 0
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.4f)
					->SetHideTabWell(true)
					->AddTab(TrainingSetDetailsTabId, ETabState::OpenedTab)
				)
#endif
			)
	);
	
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, TrainingSetEditorAppIdentifier, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, training_set);
	
	// menu customisation
//	FDataTableEditorModule& DataTableEditorModule = FModuleManager::LoadModuleChecked<FDataTableEditorModule>("DataTableEditor");
//	AddMenuExtender(DataTableEditorModule.GetMenuExtensibilityManager()->GetAllExtenders(GetToolkitCommands(), GetEditingObjects()));
	
	// toolbar customisation
	TSharedPtr<FExtender> ToolbarExtender = FInteractMLEditorModule::GetModule()->GetToolBarExtensibilityManager()->GetAllExtenders( GetToolkitCommands(), GetEditingObjects() );
	ExtendToolbar( ToolbarExtender );
	AddToolbarExtender( ToolbarExtender );
	RegenerateMenusAndToolbars();

	// Support undo/redo
	GEditor->RegisterForUndo(this);
	
	// ensure resource list set up for editing (e.g. if brand new)
	//training_set->Editor_InitForEditing();

	// prep for UI
	RebuildEntryViewModel();

#if 0
	if (DetailsView.IsValid())
	{
		//initially nothing
		DetailsView->SetObject( nullptr );
	}
#endif
}

// opportunity to add buttons to our toolbar
//
void FTrainingSetEditor::ExtendToolbar(TSharedPtr<FExtender> Extender)
{
	Extender->AddToolBarExtension(
		"Asset", EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FTrainingSetEditor::FillToolbar)
	);	
}

// populate our toolbar buttons
//
void FTrainingSetEditor::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection("TrainingSetCommands");
	{
		//delete
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FTrainingSetEditor::OnDeleteClicked),
				FCanExecuteAction::CreateSP(this, &FTrainingSetEditor::CheckDeleteAllowed)),
			NAME_None,
			LOCTEXT("TrainingSetDeleteExampleButton", "Delete"),
			LOCTEXT("TrainingSetDeleteExampleToolTip", "Delete the currently selected example(s) from the training set"),
			FSlateIcon( "InteractML", "ToolbarIcons.Delete_40x") );

		//reset
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
			FExecuteAction::CreateSP(this, &FTrainingSetEditor::OnResetClicked),
			FCanExecuteAction::CreateSP(this, &FTrainingSetEditor::CheckResetAllowed)),
			NAME_None,
			LOCTEXT("TrainingSetResetButton", "Reset"),
			LOCTEXT("TrainingSetResetToolTip", "Fully reset this training set, deletes all examples"),
			FSlateIcon( "InteractML", "ToolbarIcons.Reset_40x") );
	}
	ToolbarBuilder.EndSection();
}

// regular updates
// NOTE: used to handle deferred details panel change
//
bool FTrainingSetEditor::Tick(float DeltaTime)
{
	TrackChanges();

	return true; //keep on tickin
}

// any changes, need a refresh?
//
void FTrainingSetEditor::TrackChanges()
{
	UInteractMLTrainingSet* pexamples = GetEditableExamples();
	if (pexamples)
	{
		int then = PrevExampleCount;
		int now = pexamples->GetExampleCount();
		if (then != now)
		{
			//need a rebuild
			bool new_examples = now > then;
			RebuildEntryViewModel( new_examples );
		}
	}
}

// can we currently delete the selected example(s)?
bool FTrainingSetEditor::CheckDeleteAllowed() const
{
	if (SelectedItems.Num()==0)
	{
		return false;
	}

	for (int i = 0; i < SelectedItems.Num(); i++)
	{
		if (!CanDeleteItem( SelectedItems[i].Get() ))
		{
			return false;
		}
	}

	return true;
}


// delete the currently selected examples
//
void FTrainingSetEditor::OnDeleteClicked()
{
	UInteractMLTrainingSet* ptraining_set = GetEditableExamples();

	//undoable transaction
	const FScopedTransaction edit_delete(LOCTEXT("TrainingSetDeleteOperation","Deleting Examples"));
	ptraining_set->Modify();

	//do delete
	for (int i = 0; i < SelectedItems.Num(); i++)
	{
		UE_LOG(LogInteractML, Display, TEXT("Deleting example #%i from Training Set '%s'"), SelectedItems[i]->GetExampleID(), *GetEditableExamples()->GetName());

		int example_id = SelectedItems[i]->GetExampleID();
		ptraining_set->RemoveExample(example_id);
	}

	RebuildEntryViewModel();
}

// can we currently reset the training set?
bool FTrainingSetEditor::CheckResetAllowed() const
{
	return !GetEditableExamples()->HasBeenReset();
}

// reset the currently selected examples
//
void FTrainingSetEditor::OnResetClicked()
{
	UInteractMLTrainingSet* ptraining_set = GetEditableExamples();
	
	//undoable transaction
	const FScopedTransaction edit_reset(LOCTEXT("TrainingSetResetOperation","Resetting Training Set"));
	ptraining_set->Modify();
	
	//do delete
	ptraining_set->ResetTrainingSet();
	
	RebuildEntryViewModel();
}

// refresh internal representation of current resource list state
//
void FTrainingSetEditor::RebuildEntryViewModel( bool select_latest_example )
{
	//remember selection details
	TArray<int> selected_ids;
	for (int i = 0; i < SelectedItems.Num(); i++)
	{
		selected_ids.Add(SelectedItems[i]->GetExampleID());
	}
	int first_selected_index = -1;
	for (int i = 0; i < TreeRoots.Num(); i++)
	{
		if (selected_ids.Contains(TreeRoots[i]->GetExampleID()))
		{
			first_selected_index = i;
			break;
		}
	}

	//rebuild
	TreeRoots.Empty();

	UInteractMLTrainingSet* pexamples = GetEditableExamples();
	const TArray<FInteractMLExample>& examples = pexamples->GetExamples();

	//populate with flat table of examples
	int most_recent_id = 0;
	int most_recent_index = 0;
	for (int iexample = 0; iexample < examples.Num(); iexample++)
	{
		//each example is wrapped by a standard row object
		const FInteractMLExample& example = examples[iexample];
		FTrainingSetTreeItem* new_item = new FTrainingSetTreeItem(pexamples, iexample);
		TreeRoots.Add( MakeShareable(new_item) );

		//track most recent
		if (new_item->ExampleID > most_recent_id)
		{
			most_recent_id = new_item->ExampleID;
			most_recent_index = iexample;
		}
	}

	//restore selection
	TArray<FTrainingSetTreeItem::Ptr> items_to_select;
	if (select_latest_example)
	{
		//want most recently added
		FTrainingSetTreeItem::Ptr item = FindTreeItemByIndex( most_recent_index );
		if (item.IsValid())
		{
			items_to_select.Add(item);
		}
	}
	else
	{
		//attempt to restore previously selected elements
		int restored = 0;
		for (int id : selected_ids)
		{
			FTrainingSetTreeItem::Ptr item = FindTreeItemByExampleID(id);
			if (item.IsValid())
			{
				items_to_select.Add(item);
				restored++;
			}
		}
		//if none (e.g. deleted) then at least try to select similar location in table
		if (restored == 0)
		{
			FTrainingSetTreeItem::Ptr item = FindTreeItemByIndex(first_selected_index);
			if (item.IsValid())
			{
				items_to_select.Add(item);
			}
		}
	}
	//apply
	TreeView->ClearSelection();
	TreeView->SetItemSelection(items_to_select, true);
	if(select_latest_example && items_to_select.Num()>0)
	{
		TreeView->RequestScrollIntoView( items_to_select[0] );
	}
	
	//apply to UI
	RefreshTreeview();

	//up to date
	PrevExampleCount = pexamples->GetExampleCount();
}

// recursive search of all items for item with this example id
//
FTrainingSetTreeItem::Ptr FTrainingSetEditor::FindTreeItemByExampleID(int example_id)
{
	for (int i = 0; i < TreeRoots.Num(); i++)
	{
		FTrainingSetTreeItem::Ptr p = FindTreeItemByExampleID(TreeRoots[i], example_id);
		if (p.IsValid())
		{
			return p;
		}
	}
	return nullptr;
}

// recursive search for item with this example id
//
FTrainingSetTreeItem::Ptr FTrainingSetEditor::FindTreeItemByExampleID(FTrainingSetTreeItem::Ptr p, int example_id)
{
	//check me
	if (p->GetExampleID() == example_id)
	{
		return p;
	}

	//recurse to children
	for (int i = 0; i < p->Children.Num(); i++)
	{
		FTrainingSetTreeItem::Ptr q = FindTreeItemByExampleID(p->Children[i], example_id);
		if (q.IsValid())
		{
			return q;
		}
	}

	return nullptr;
}

// recursive search of all items for item with this index
//
FTrainingSetTreeItem::Ptr FTrainingSetEditor::FindTreeItemByIndex(int example_index)
{
	int current_index = 0;
	for (int i = 0; i < TreeRoots.Num(); i++)
	{
		FTrainingSetTreeItem::Ptr p = FindTreeItemByIndex(TreeRoots[i], example_index, current_index);
		if (p.IsValid())
		{
			return p;
		}
	}
	return nullptr;
}

// recursive search for item with this specific index
//
FTrainingSetTreeItem::Ptr FTrainingSetEditor::FindTreeItemByIndex(FTrainingSetTreeItem::Ptr p, int example_index, int& current_index)
{
	//check me
	if(current_index == example_index)
	{
		return p;
	}
	
	//it's not me, maybe next
	current_index++;
	
	//recurse to children
	for (int i = 0; i < p->Children.Num(); i++)
	{
		FTrainingSetTreeItem::Ptr q = FindTreeItemByIndex(p->Children[i], example_index, current_index);
		if (q.IsValid())
		{
			return q;
		}
	}
	return nullptr;
}


// update Slate-side when our viewmodel has chnaged
//
void FTrainingSetEditor::RefreshTreeview()
{
	//trigger UI update to match
	TreeView->RebuildList(); //ensure any item presentation changes are applied
	TreeView->RequestTreeRefresh(); //update tree UI structure from hierarchy
}


/** IToolkit interface */

FName FTrainingSetEditor::GetToolkitFName() const
{
	return FName("InteractMLTrainingSetEditor");
}

FText FTrainingSetEditor::GetBaseToolkitName() const
{
	return LOCTEXT( "InteractMLTrainingSetEditorLabel", "Training Set Editor" );
}

FString FTrainingSetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("InteractMLTrainingSetEditorTabPrefix", "Training Set ").ToString();
}

FLinearColor FTrainingSetEditor::GetWorldCentricTabColorScale() const
{
	return FColor::Emerald.ReinterpretAsLinear();
}

/** FEditorUndoClient */

void FTrainingSetEditor::PostUndo( bool bSuccess )
{
	OnUndoRedo();
}
void FTrainingSetEditor::PostRedo( bool bSuccess )
{
	OnUndoRedo();
}

// action required after an editor-wide undo or a redo
//
void FTrainingSetEditor::OnUndoRedo()
{
	//invalidates view model
	RebuildEntryViewModel();

	//potentially invalidates all entities
	//FInteractMLModule::GetModule()->Editor_Notify??Changed();
}


/** FAssetEditorToolkit interface */

void FTrainingSetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_TrainingSetEditor", "Training Set Editor"));
	
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	CreateAndRegisterTrainingSetHierarchyTab(InTabManager);
	//CreateAndRegisterTrainingSetDetailsTab(InTabManager);
}

void FTrainingSetEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	InTabManager->UnregisterTabSpawner(TrainingSetHierarchyTabId);
	//InTabManager->UnregisterTabSpawner(TrainingSetDetailsTabId);
	
	TrainingSetHierarchyTabWidget.Reset();
}

// hierarchy tab type
//
void FTrainingSetEditor::CreateAndRegisterTrainingSetHierarchyTab(const TSharedRef<class FTabManager>& InTabManager)
{
	TrainingSetHierarchyTabWidget = CreateTrainingSetHierarchyUI();
	
	InTabManager->RegisterTabSpawner(TrainingSetHierarchyTabId, FOnSpawnTab::CreateSP(this, &FTrainingSetEditor::SpawnTab_TrainingSetHierarchy))
		.SetDisplayName(LOCTEXT("TrainingSetHierarchyTab", "Examples"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

#if 0
// details tab type
//
void FTrainingSetEditor::CreateAndRegisterTrainingSetDetailsTab(const TSharedRef<class FTabManager>& InTabManager)
{
	//access standard property panel
	FPropertyEditorModule & EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(/*bUpdateFromSelection=*/ false, /*bLockable=*/ false, /*bAllowSearch=*/ true, /*InNameAreaSettings=*/ FDetailsViewArgs::HideNameArea, /*bHideSelectionTip=*/ true);
	DetailsView = EditModule.CreateDetailView(DetailsViewArgs);
	
	InTabManager->RegisterTabSpawner(TrainingSetDetailsTabId, FOnSpawnTab::CreateSP(this, &FTrainingSetEditor::SpawnTab_TrainingSetDetails))
		.SetDisplayName(LOCTEXT("TrainingSetDetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}
#endif

// hierarchy tab creation
//
TSharedRef<SDockTab> FTrainingSetEditor::SpawnTab_TrainingSetHierarchy( const FSpawnTabArgs& Args )
{
	check( Args.GetTabId().TabType == TrainingSetHierarchyTabId );

	UInteractMLTrainingSet* res_list = GetEditableExamples();

	// Support undo/redo
	if (res_list)
	{
		res_list->SetFlags(RF_Transactional);
	}

	TSharedRef<SDockTab> tab = SNew(SDockTab)
		//.Icon( FEditorStyle::GetBrush("GenericEditor.Tabs.Properties") )
		.Icon( FEditorStyle::GetBrush("LevelEditor.Tabs.ContentBrowser") )
		.Label( LOCTEXT("TrainingSetHierarcyTitle", "Examples") )
		.TabColorScale( GetTabColorScale() )
		//.OnTabActivated( this, &FTrainingSetEditor::OnHierarchyTabActivated )	//this doesn't appear to work, see below
		[
			SNew(SBorder)
			.Padding(2)
			.BorderImage( FEditorStyle::GetBrush( "ToolPanel.GroupBorder" ) )
			[
				TrainingSetHierarchyTabWidget.ToSharedRef()
			]
		];

	//BUG?: the imperitive event setter above doesn't appear to work, direct setter does however
	tab->SetOnTabActivated( SDockTab::FOnTabActivatedCallback::CreateSP( this, &FTrainingSetEditor::OnHierarchyTabActivated ) );

	return tab;
}

#if 0
// property tab creation
//
TSharedRef<SDockTab> FTrainingSetEditor::SpawnTab_TrainingSetDetails(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId().TabType == ResourceListDetailsTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.Label(LOCTEXT("ResourceListDetailsTitle", "Details"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SBorder)
			.Padding(2)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				DetailsView.ToSharedRef()
			]
		];
}
#endif

// hierarchy tab active, ensure property panel follows selected item
//
void FTrainingSetEditor::OnHierarchyTabActivated(TSharedRef<SDockTab> tab, ETabActivationCause cause)
{
}

//text access helpers
static FText GetExampleModeText(UInteractMLTrainingSet* pexamples)
{
	if (pexamples->IsSingleSamples())
	{
		return LOCTEXT("TrainingSetModeDescSingle", "Single");
	}
	else if (pexamples->IsSeriesSamples())
	{
		return LOCTEXT("TrainingSetModeDescSeries", "Series");
	}
	else
	{
		return LOCTEXT("TrainingSetModeDescUnknown", "N/A");
	}
}
static FText GetLabelTypeText(UInteractMLTrainingSet* pexamples)
{
	const class UInteractMLLabel* label_type = pexamples->GetLabelCache().LabelType;
	if (label_type)
	{
		return FText::FromString( label_type->GetName() );
	}
	else
	{
		return LOCTEXT("TrainingSetLabelTypeSimpleDescription", "Number");
	}
}
static FText GetExampleCountTooltip(UInteractMLTrainingSet* pexamples)
{
	return LOCTEXT("TrainingSetExampleCountTooltip", "Number of examples recorded into this training set");
}
static FText GetExampleModeTooltip(UInteractMLTrainingSet* pexamples)
{
	if (pexamples->IsSingleSamples())
	{
		return LOCTEXT("TrainingSetModeTooltipSingle", "A single parameter snapshot is recorded with each example");
	}
	else if (pexamples->IsSeriesSamples())
	{
		return LOCTEXT("TrainingSetModeTooltipSeries", "A series of parameter snapshots are recorded for each example, usually over time");
	}
	else
	{
		return LOCTEXT("TrainingSetModeTooltipUnknown", "No examples recorded, mode unknown");
	}
}
static FText GetExampleParametersTooltip(UInteractMLTrainingSet* pexamples)
{
	return LOCTEXT("TrainingSetParametersTooltip", "The number of numerical values derived from the live parameters recorded with each sample");
}
static FText GetExampleLabelCountTooltip(UInteractMLTrainingSet* pexamples)
{
	return LOCTEXT("TrainingSetLabelCountTooltip", "The number of distinct labels (output states) that examples have been recorded for");
}
static FText GetExampleLabelTypeTooltip(UInteractMLTrainingSet* pexamples)
{
	const class UInteractMLLabel* label_type = pexamples->GetLabelCache().LabelType;
	if (label_type)
	{
		return LOCTEXT("TrainingSetLabelTypeCompositeTooltip", "Label is a specific composite type, comprising of one or more assorted data types, see asset for details");
	}
	else
	{
		return LOCTEXT("TrainingSetLabelTypeSimpleTooltip", "Label is a single numerical (float) value");
	}
}

// parameterically access the above property info (so they are dynamic)
//
FText FTrainingSetEditor::GetPropertyText(UInteractMLTrainingSet* pexamples, EInteractMLTrainingSetProperty property_type) const
{
	switch (property_type)
	{
		case EInteractMLTrainingSetProperty::ExampleCount:
			return FText::FromString(FString::FromInt(pexamples->GetExamples().Num()));
		case EInteractMLTrainingSetProperty::SampleMode:
			return GetExampleModeText(pexamples);
		case EInteractMLTrainingSetProperty::LabelCount:
			return FText::FromString(FString::FromInt(pexamples->GetLabelCount()));
		case EInteractMLTrainingSetProperty::ParameterCount:
			return FText::FromString(FString::FromInt(pexamples->GetParameterCount()));
		case EInteractMLTrainingSetProperty::LabelType:
			return GetLabelTypeText(pexamples);
	}
	return FText::FromString("");
}


// hierarchy UI creation
//
TSharedRef<SVerticalBox> FTrainingSetEditor::CreateTrainingSetHierarchyUI()
{
	UInteractMLTrainingSet* pexamples = GetEditableExamples();

	return SNew(SVerticalBox)
		// training set information
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.Padding(FMargin(3))
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
			[
				//(icon) Training Set Name
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5, 2, 5, 2)
				.VAlign( VAlign_Center )
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SScaleBox)
						.StretchDirection(EStretchDirection::DownOnly)
						.VAlign( VAlign_Center )
						[
							SNew(SImage)
							.Image(	FInteractMLEditorModule::GetStyle()->GetBrush("MiscIcons.TrainingSet_16x") )
						]
					]
					+ SHorizontalBox::Slot()
					.Padding(4,0,0,0)
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text( FText::FromString( pexamples->GetName() ) )
						.Font(FCoreStyle::GetDefaultFontStyle("Normal", 16))
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					//Example Count
					+SHorizontalBox::Slot()
					.MaxWidth(180.0f)
					[
						CreateInfoField(LOCTEXT("TrainingSetExampleCountLabel", "Examples"), pexamples, EInteractMLTrainingSetProperty::ExampleCount, GetExampleCountTooltip( pexamples ))
					]
					//Sample Mode
					+SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						CreateInfoField(LOCTEXT("TrainingSetModeLabel", "Sample Mode"), pexamples, EInteractMLTrainingSetProperty::SampleMode, GetExampleModeTooltip( pexamples ), 500.0f)
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					//Label Count
					+SHorizontalBox::Slot()
					.MaxWidth(180.0f)
					[
						CreateInfoField(LOCTEXT("TrainingSetLabelCountLabel", "Labels"), pexamples, EInteractMLTrainingSetProperty::LabelCount, GetExampleLabelCountTooltip( pexamples ))
					]
					//Parameter Count
					+SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						CreateInfoField(LOCTEXT("TrainingSetParametersLabel", "Parameters"), pexamples, EInteractMLTrainingSetProperty::ParameterCount, GetExampleParametersTooltip( pexamples ), 500.0f)
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					//Label Type
					+SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						CreateInfoField(LOCTEXT("TrainingSetLabelTypeLabel", "Label Type"), pexamples, EInteractMLTrainingSetProperty::LabelType, GetExampleLabelTypeTooltip( pexamples ), 1000.0f )
					]
				]
			]
		]

		//examples table
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(TreeView, STrainingSetTreeView, SharedThis(this))
			.TreeItemsSource(&TreeRoots)
			.OnGenerateRow(this, &FTrainingSetEditor::GenerateTreeRow)
//			.OnItemScrolledIntoView(TreeView, &STrainingSetTreeView::OnTreeItemScrolledIntoView)
			.ItemHeight(21)
//			.SelectionMode(InArgs._SelectionMode)
			.OnSelectionChanged(this, &FTrainingSetEditor::OnTreeViewSelect)
//			.OnExpansionChanged(this, &SPathView::TreeExpansionChanged)
			.OnGetChildren(this, &FTrainingSetEditor::GetChildrenForTree)
//			.OnSetExpansionRecursive(this, &SPathView::SetTreeItemExpansionRecursive)
//			.OnContextMenuOpening(this, &FTrainingSetEditor::GenerateTreeContextMenu)
			.ClearSelectionOnClick(false)
			.HighlightParentNodesForSelection(true)				
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Group() )
				.FillWidth(0.05f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupColumnTitle","") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Number() )
				.FillWidth(0.05f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupNumberTitle","#") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Label() )
				.FillWidth(0.1f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupLabelTitle","Label") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Samples() )
				.FillWidth(0.05f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupSamplesTitle","Samples") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Duration() )
				.FillWidth(0.05f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupDurationTitle","Duration") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Session() )
				.FillWidth(0.1f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupSessionTitle","Session") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::User() )
				.FillWidth(0.1f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupUserTitle","User") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Notes() )
				.FillWidth(0.2f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetGroupNotesTitle","Notes") )
					.Margin(4.0f)
				]
			)
		];
}

// build a name/value property display item
//
TSharedRef<SWidget> FTrainingSetEditor::CreateInfoField(FText name, UInteractMLTrainingSet* pexamples, EInteractMLTrainingSetProperty property_type, FText tooltip, float max_text_width)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.Padding(5, 2, 5, 2)
		.FillWidth(3.0f)
		.MaxWidth(85.0f)
		.VAlign( VAlign_Center )
		[
			SNew(STextBlock)
			.Text(FText::Format(LOCTEXT("TrainingSetInfoLabelFormat", "{0}:"), { name }))
			.ToolTipText(tooltip)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.MaxWidth( max_text_width )
		.VAlign( VAlign_Center )
		[
			SNew(STextBlock)
			.Text_Raw( this, &FTrainingSetEditor::GetPropertyText, pexamples, property_type )
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
			.ToolTipText(tooltip)
		];
}

#if 0
// context menu on items
//
TSharedPtr<SWidget> FTrainingSetEditor::GenerateTreeContextMenu()
{
	return nullptr;
}
#endif

// hierarchy item selected
//
void FTrainingSetEditor::OnTreeViewSelect( FTrainingSetTreeItem::Ptr Item, ESelectInfo::Type SelectInfo )
{
	//refresh selected item list
	TreeView->GetSelectedItems(SelectedItems);
}

// is user allowed to delete this item?
//
bool FTrainingSetEditor::CanDeleteItem( FTrainingSetTreeItem* item ) const
{
	return item!=nullptr;
}
	
// can we start a drag and drop operation from this item?
//
bool FTrainingSetEditor::IsItemValidDragSource( FTrainingSetTreeItem* item ) const
{
	//can move any sub-entry of the resource hierarchy
	return false;
}


	

// build a row on demand
//
TSharedRef<ITableRow> FTrainingSetEditor::GenerateTreeRow( TSharedPtr<FTrainingSetTreeItem> TreeItem, const TSharedRef<STableViewBase>& OwnerTable )
{
	check(TreeItem.IsValid());
	return SNew(STrainingSetTreeRow, OwnerTable)
		.Item( TreeItem )
		.TrainingSetEditor(SharedThis(this));
}

// hierarchy access
//
void FTrainingSetEditor::GetChildrenForTree( TSharedPtr< FTrainingSetTreeItem > TreeItem, TArray< TSharedPtr<FTrainingSetTreeItem> >& OutChildren )
{
	//TreeItem->SortChildrenIfNeeded();
	OutChildren = TreeItem->Children;
}


//------------------------------------------------------------------------
// STrainingSetTreeView

void STrainingSetTreeView::Construct(const FArguments& InArgs, TSharedRef<FTrainingSetEditor> Owner)
{
	TrainingSetEditorWeak = Owner;
	STreeView::Construct( InArgs );
}

// key handling
//	F2 - rename
//	Del - delete
//
FReply STrainingSetTreeView::OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent )
{
	//our editor
	TSharedPtr<FTrainingSetEditor> training_set_editor = TrainingSetEditorWeak.Pin();
	if(training_set_editor.IsValid())
	{		
		//delete/bs to delete entries	
		if ( InKeyEvent.GetKey() == EKeys::Delete || InKeyEvent.GetKey() == EKeys::BackSpace )
		{
			training_set_editor->OnDeleteClicked();
			return FReply::Handled();
		}
	}

	return STreeView<TSharedPtr<FTrainingSetTreeItem>>::OnKeyDown( MyGeometry, InKeyEvent );
}


//------------------------------------------------------------------------
// FTrainingSetDragDrop setup

FTrainingSetDragDrop::FTrainingSetDragDrop( FTrainingSetTreeItem::Ptr dragged_item )
	: Item( dragged_item )
{
}

void FTrainingSetDragDrop::Construct()
{
	FDecoratedDragDropOp::Construct();

	CurrentHoverText = FText::FromString( Item->GetLabel() );
	CurrentIconBrush = Item->GetIcon( false );
	SetDecoratorVisibility( true );
}

//------------------------------------------------------------------------
// STrainingSetItemDropTarget

void STrainingSetItemDropTarget::Construct(const FArguments& InArgs )
{
	OnAssetDropped = InArgs._OnAssetDropped;
	OnIsAssetAcceptableForDrop = InArgs._OnIsAssetAcceptableForDrop;

	OnItemDropped = InArgs._OnItemDropped;
	OnIsItemAcceptableForDrop = InArgs._OnIsItemAcceptableForDrop;
	
	SDropTarget::Construct(
		SDropTarget::FArguments()
		.OnDrop(this, &STrainingSetItemDropTarget::OnDropped)
		[
		InArgs._Content.Widget
		]);
}

// drop operation entry point
//
FReply STrainingSetItemDropTarget::OnDropped(TSharedPtr<FDragDropOperation> DragDropOperation)
{
	bool bUnused;

	// asset drop case
	UObject* Object = GetDroppedObject(DragDropOperation, bUnused);	
	if ( Object )
	{
		OnAssetDropped.ExecuteIfBound(Object);
	}

	// item drop case
	FTrainingSetTreeItem::Ptr Item = GetDroppedItem(DragDropOperation, bUnused);
	if( Item.IsValid())
	{
		OnItemDropped.ExecuteIfBound(Item);
	}
	
	return FReply::Handled();
}

// is this an allowed drop? (even if it's a valid type)
//
bool STrainingSetItemDropTarget::OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation) const
{
	bool bUnused = false;

	// asset drop case	
	UObject* Object = GetDroppedObject(DragDropOperation, bUnused);
	if ( Object )
	{
		// Check and see if its valid to drop this object
		if ( OnIsAssetAcceptableForDrop.IsBound() )
		{
			return OnIsAssetAcceptableForDrop.Execute(Object);
		}
		else
		{
			// If no delegate is bound assume its always valid to drop this object
			return true;
		}
	}

	// item drop case
	FTrainingSetTreeItem::Ptr Item = GetDroppedItem(DragDropOperation, bUnused);
	if( Item.IsValid())
	{
		// Check and see if its valid to drop this object
		if ( OnIsItemAcceptableForDrop.IsBound() )
		{
			return OnIsItemAcceptableForDrop.Execute(Item);
		}
		else
		{
			// If no delegate is bound assume its always valid to drop this object
			return true;
		}
	}
	
	return false;
}

// is the drop payload something we are interested in?
//
bool STrainingSetItemDropTarget::OnIsRecognized(TSharedPtr<FDragDropOperation> DragDropOperation) const
{
	bool bRecognizedEventO = false;
	bool bRecognizedEventI = false;

	UObject* Object = GetDroppedObject(DragDropOperation, bRecognizedEventO);
	FTrainingSetTreeItem::Ptr Item = GetDroppedItem(DragDropOperation, bRecognizedEventI);
	
	return bRecognizedEventO || bRecognizedEventI;
}

// extract the asset being dropped (if it is one)
//
UObject* STrainingSetItemDropTarget::GetDroppedObject(TSharedPtr<FDragDropOperation> DragDropOperation, bool& bOutRecognizedEvent) const
{
	bOutRecognizedEvent = false;
	UObject* DroppedObject = NULL;
	
	// Asset being dragged from content browser
	if ( DragDropOperation->IsOfType<FAssetDragDropOp>() )
	{
		bOutRecognizedEvent = true;
		TSharedPtr<FAssetDragDropOp> DragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(DragDropOperation);
		const TArray<FAssetData>& DroppedAssets = DragDropOp->GetAssets();
		
		bool bCanDrop = DroppedAssets.Num() == 1;
		
		if( bCanDrop )
		{
			const FAssetData& AssetData = DroppedAssets[0];
			
			// Make sure the asset is loaded
			DroppedObject = AssetData.GetAsset();
		}
	}

	return DroppedObject;
}

// extract the item being dropped (if it is one)
//
FTrainingSetTreeItem::Ptr STrainingSetItemDropTarget::GetDroppedItem(TSharedPtr<FDragDropOperation> DragDropOperation, bool& bOutRecognizedEvent) const
{
	bOutRecognizedEvent = false;
	FTrainingSetTreeItem::Ptr DroppedItem;
	
	// Item being dragged from elsewhere in resource list editor
	if ( DragDropOperation->IsOfType<FTrainingSetDragDrop>() )
	{
		TSharedPtr<FTrainingSetDragDrop> DragDropOp = StaticCastSharedPtr<FTrainingSetDragDrop>(DragDropOperation);
		if(DragDropOp->Item.IsValid())
		{
			bOutRecognizedEvent = true;

			//extract item from 
			DroppedItem = DragDropOp->Item;
		}
	}
	
	return DroppedItem;
}


//------------------------------------------------------------------------
// STrainingSetTreeRow

void STrainingSetTreeRow::Construct( const FArguments& InArgs, const TSharedRef<STableViewBase>& TreeView )
{
	Item = InArgs._Item->AsShared();
	TrainingSetEditorWeak = InArgs._TrainingSetEditor;

	auto Args = FSuperRowType::FArguments()
		.Style(&FEditorStyle::Get().GetWidgetStyle<FTableRowStyle>("SceneOutliner.TableViewRow"));
	Args.OnDragDetected( this, &STrainingSetTreeRow::OnDragDetected );
	
	SMultiColumnTableRow< TSharedPtr<FTrainingSetTreeItem>>::Construct(Args, TreeView);
}

// watch for mouse down so we can detect drag operations beginning
//
FReply STrainingSetTreeRow::OnMouseButtonDown( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent )
{
	auto ItemPtr = Item.Pin();
	if (ItemPtr.IsValid())
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			FReply Reply = SMultiColumnTableRow<TSharedPtr<FTrainingSetTreeItem>>::OnMouseButtonDown( MyGeometry, MouseEvent );
			
			if (TrainingSetEditorWeak.Pin()->IsItemValidDragSource( ItemPtr.Get() ))
			{
				return Reply.DetectDrag( SharedThis(this) , EKeys::LeftMouseButton );
			}
			
			return Reply.PreventThrottling();
		}
	}
	
	return FReply::Handled();
}

// kick off a drag operation
//
FReply STrainingSetTreeRow::OnDragDetected( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent )
{
	auto ItemPtr = Item.Pin();
	if(MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton )
		&& TrainingSetEditorWeak.Pin()->IsItemValidDragSource( ItemPtr.Get() ))
	{
		//build drag payload
		TSharedPtr<FTrainingSetDragDrop> dnd = MakeShareable( new FTrainingSetDragDrop( ItemPtr ) );
		return FReply::Handled().BeginDragDrop( dnd.ToSharedRef() );
	}

	return FReply::Unhandled();
}



FReply STrainingSetTreeRow::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}

FReply STrainingSetTreeRow::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	//target (this)
	auto pdestination_item = Item.Pin();

	//operation
	TSharedPtr<FDragDropOperation> op = DragDropEvent.GetOperation();
	if(op->IsOfType<FTrainingSetDragDrop>())
	{
		FTrainingSetDragDrop* psource = (FTrainingSetDragDrop*)op.Get();
		if(psource)
		{
			UE_LOG(LogInteractML,Display,TEXT("DROPPED ITEM '%s' ONTO '%s'"), *psource->Item->GetLabel(), *pdestination_item->GetLabel() );
		}
	}
	return FReply::Handled();
}

// Row accessor handler: get name
//
FText STrainingSetTreeRow::GetRowName( FTrainingSetTreeItem::Ptr item ) const
{
	return FText::FromString( TEXT("") );
}

// Row accessor handler: get icon
//
const FSlateBrush* STrainingSetTreeRow::GetRowIcon( FTrainingSetTreeItem::Ptr item ) const
{
	return item->GetIcon( false );//item->Flags.bIsExpanded );
}

// Row content dimming for variants
//
FSlateColor STrainingSetTreeRow::GetRowColourAndOpacity( FTrainingSetTreeItem::Ptr item ) const
{
	if(item->Examples->IsSingleSamples())
	{
		return FSlateColor::UseSubduedForeground(); //dim columns not used by single sample recordings
	}
	else
	{
		//normal appearance when series recording
		return FSlateColor::UseForeground();
	}	
}

// Row accessor handler: get resource
//
UObject* STrainingSetTreeRow::GetRowResource( FTrainingSetTreeItem::Ptr item ) const
{
	return nullptr; //item->GetResource();
}

// Row accessor handler: set resources
void STrainingSetTreeRow::SetRowResource( UObject* new_value, FTrainingSetTreeItem::Ptr item )
{
#if 0
	if(item->SetResource( new_value ))
	{
		//update treeview (in case assignment changed semantics of controls, e.g. not want specific type filter)
		TSharedPtr<FTrainingSetEditor> plisteditor = TrainingSetEditorWeak.Pin();
		plisteditor->RebuildEntryViewModel();
	}
#endif
}

// Build UI for a specific column of the hierarchy
//
TSharedRef<SWidget> STrainingSetTreeRow::GenerateWidgetForColumn( const FName& ColumnName )
{
	FTrainingSetTreeItem::Ptr ItemPtr = Item.Pin();
	if (!ItemPtr.IsValid())
	{
		return SNullWidget::NullWidget;
	}

	//------------------------------------------------------------------------
	// grouping label, icon and hierarchy
	if( ColumnName == STrainingSetTreeView::ColumnTypes::Group() )
	{
		TSharedPtr<FTrainingSetEditor> plisteditor = TrainingSetEditorWeak.Pin();

		//editable text (to support rename)
		TSharedPtr<SInlineEditableTextBlock> InlineTextBlock = SNew(SInlineEditableTextBlock)
			.Text(this, &STrainingSetTreeRow::GetRowName, ItemPtr )
//			.HighlightText( SceneOutliner.GetFilterHighlightText() )
//			.ColorAndOpacity(this, &STrainingSetTreeRow::GetRowColourAndOpacity, ItemPtr);
//			.OnTextCommitted(this, &STrainingSetTreeRow::OnItemLabelCommitted, ItemPtr)
//			.OnVerifyTextChanged(this, &STrainingSetTreeRow::OnVerifyItemLabelChange, ItemPtr)
//			.IsSelected(FIsSelected::CreateSP(&InRow, &STrainingSetTreeRow::IsSelectedExclusively))
//			.IsReadOnly_Lambda([Editor = plisteditor, Item = ItemPtr, this]()
//			{
//				return !Editor->CanRenameItem( Item.Get() );
//			});		
//		if(plisteditor->CanRenameItem( ItemPtr.Get() ))
//		{
//			ItemPtr->RenameRequestEvent.BindSP( InlineTextBlock.Get(), &SInlineEditableTextBlock::EnterEditingMode );
//		}			
			;

		// The first column gets the tree expansion arrow for this row
		auto ui =
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(6, 0, 0, 0)
			[
				SNew( SExpanderArrow, SharedThis(this) ).IndentAmount(12)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.FillWidth(1.0f)
			[
				SNew( STrainingSetItemDropTarget )
				.OnIsAssetAcceptableForDrop( this, &STrainingSetTreeRow::OnAssetDraggedOverName )
				.OnAssetDropped( this, &STrainingSetTreeRow::OnAssetDroppedName )
				.OnIsItemAcceptableForDrop( this, &STrainingSetTreeRow::OnItemDraggedOverName )
				.OnItemDropped( this, &STrainingSetTreeRow::OnItemDroppedName )
				[
					SNew( SHorizontalBox )
					+SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(SImage)
						.Image(this, &STrainingSetTreeRow::GetRowIcon, ItemPtr )
					]
					+SHorizontalBox::Slot()
					.Padding(6,0,0,0)
					.VAlign(VAlign_Center)
					.FillWidth(1.0f)
					[ 
						InlineTextBlock.ToSharedRef()
//						SNew( STextBlock )
//						.Text( this, &STrainingSetTreeRow::GetRowName, ItemPtr )
					]
				]
			];

			return ui;
	}
	//------------------------------------------------------------------------
	// number
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Number() )
	{
		// The second column displays the number for the item
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( FText::FromString( FString::FromInt( ItemPtr->GetExampleID() ) ) )
			];
	}	
	//------------------------------------------------------------------------
	// label
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Label())
	{
		// The third column displays the label for this example
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( FText::FromString( ItemPtr->GetLabel() ) )
			];
	}
	//------------------------------------------------------------------------
	// Samples
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Samples())
	{
		// This column show sample count (only applicable to Series recording)
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( FText::FromString( FString::FromInt( ItemPtr->GetSamples() ) ) )
				.ColorAndOpacity(this, &STrainingSetTreeRow::GetRowColourAndOpacity, ItemPtr)
		];
	}
	//------------------------------------------------------------------------
	// Duration
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Duration())
	{
		// This column show recording duration time (only applicable to Series recording)
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( ItemPtr->GetDurationText() )
				.ColorAndOpacity(this, &STrainingSetTreeRow::GetRowColourAndOpacity, ItemPtr)
			];
	}
	//------------------------------------------------------------------------
	// Session
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Session())
	{
		// This column show session recording timestamp
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( ItemPtr->GetSessionText() )
			];
	}
	//------------------------------------------------------------------------
	// User
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::User())
	{
		// This column shows the user who recorded this
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( ItemPtr->GetUser() )
			];
	}
	//------------------------------------------------------------------------
	// Notes
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Notes())
	{
		// This column shows any notes attached
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( FText::FromString("") )
			];
	}
	else
	{
		//no editing UI needed for folders/root
		return SNullWidget::NullWidget;
	}
}

// asset dragged over hierarchy entry
//
bool STrainingSetTreeRow::OnAssetDraggedOverName( const UObject* asset )
{
#if 0
	//source
	const UClass* asset_class = asset->GetClass();

	//target
	FTrainingSetTreeItem::Ptr item = Item.Pin();

	TSharedPtr<FTrainingSetEditor> res_list_editor = TrainingSetEditorWeak.Pin();
	const UClass* entry_class = res_list_editor->GetEntryTypeForAsset( asset_class );
	if(entry_class) //only supported types
	{
		FTrainingSetInsertion insertion = res_list_editor->EvaluateAdd( item.Get(), entry_class );
		if(insertion.IsValid())
		{
			//UE_LOG(LogInteractML,Display,TEXT("ASSET %s DRAGGED OVER %s"), *asset->GetName(), *item->GetName() );
			return true;
		}
	}
#endif
	return false;
}

// asset dropped on hierarchy entry
//
void STrainingSetTreeRow::OnAssetDroppedName( UObject* asset )
{
#if 0
	//source
	const UClass* asset_class = asset->GetClass();

	//target
	FTrainingSetTreeItem::Ptr item = Item.Pin();

	TSharedPtr<FTrainingSetEditor> res_list_editor = TrainingSetEditorWeak.Pin();
	const UClass* entry_class = res_list_editor->GetEntryTypeForAsset( asset_class );
	if(entry_class) //only supported types
	{
		FTrainingSetInsertion insertion = res_list_editor->EvaluateAdd( item.Get(), entry_class );
		if(insertion.IsValid())
		{
			UE_LOG(LogInteractML,Display,TEXT("ASSET %s DROPPED ON %s ENTRY"), *asset->GetName(), *item->GetName() );
			res_list_editor->InsertEntry( insertion, asset );
		}
	}
#endif
}

// item dragged over entry slot from somewhere else in the hierarchy
//
bool STrainingSetTreeRow::OnItemDraggedOverName( FTrainingSetTreeItem::Ptr source_item )
{
#if 0
	//target
	FTrainingSetTreeItem::Ptr target_item = Item.Pin();

	TSharedPtr<FTrainingSetEditor> res_list_editor = TrainingSetEditorWeak.Pin();
	FTrainingSetMovement movement = res_list_editor->EvaluateMove( source_item.Get(), target_item.Get() );
	if(movement.IsValid())
	{
		//UE_LOG(LogInteractML,Display,TEXT("ASSET %s DRAGGED OVER %s"), *asset->GetName(), *item->GetName() );
		return true;
	}
#endif
	return false;
}

// item dropped on entry slot from somewhere else in the hierarchy
//
void STrainingSetTreeRow::OnItemDroppedName( FTrainingSetTreeItem::Ptr source_item )
{
#if 0
	//target
	FTrainingSetTreeItem::Ptr target_item = Item.Pin();
	
	TSharedPtr<FTrainingSetEditor> res_list_editor = ResourceListEditorWeak.Pin();
	FResourceListMovement movement = res_list_editor->EvaluateMove( source_item.Get(), target_item.Get() );
	if(movement.IsValid())
	{
		UE_LOG(LogInteractML,Display,TEXT("ITEM %s DROPPED ON %s ENTRY"), *source_item->GetName(), *target_item->GetName() );
		res_list_editor->MoveEntry( movement );
	}
#endif
}


// asset dragged over entry slot
//
bool STrainingSetTreeRow::OnAssetDraggedOverSlot( const UObject* asset )
{
#if 0
	TSharedPtr<FTrainingSetEditor> res_list_editor = ResourceListEditorWeak.Pin();

	//source
	const UClass* asset_class = asset->GetClass();

	//source must be one we can support
	if(res_list_editor->GetEntryTypeForAsset( asset_class )==nullptr)
	{
		return false;
	}
	
	//target
	FTrainingSetTreeItem::Ptr item = Item.Pin();
	if(item->GetAssetType()==nullptr)
	{
		return false;
	}

	//yup, we can drop, even if it means converting the type
	return true;
#else
	return false;
#endif
}

// asset dropped on entry slot
//
void STrainingSetTreeRow::OnAssetDroppedSlot( UObject* asset )
{
#if 0
	//target
	FTrainingSetTreeItem::Ptr item = Item.Pin();
	
//	UE_LOG(LogInteractML,Display,TEXT("ASSET %s DROPPED ON %s SLOT"), *asset->GetName(), *item->GetName() );
	if(item->SetResource( asset ))
	{
		//update treeview (in case assignment changed semantics of controls, e.g. not want specific type filter)
		TSharedPtr<FTrainingSetEditor> plisteditor = ResourceListEditorWeak.Pin();
		plisteditor->RebuildEntryViewModel();
	}
#endif
}
