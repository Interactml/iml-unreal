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
#include "WorkflowOrientedApp/SContentReference.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "SAssetDropTarget.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

//module
#include "InteractMLTrainingSet.h"
#include "InteractMLEditor.h"

#define LOCTEXT_NAMESPACE "InteractML"


const FName FTrainingSetEditor::TrainingSetEditorAppIdentifier( TEXT( "InteractMLTrainingSetEditorApp" ) );
const FName FTrainingSetEditor::TrainingSetHierarchyTabId("InteractMLTrainingSetEditor_Hierarchy");
//const FName FTrainingSetEditor::TrainingSetDetailsTabId("InteractMLTrainingSetEditor_Details");




//////////////////////////////////////////////////////////////////////////
// FTrainingSetTreeItem

FString FTrainingSetTreeItem::GetName() const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
	if(pexamples)
	{
		return pexamples->GetName();
	}
	
	return TEXT("???");
}

const FSlateBrush* FTrainingSetTreeItem::GetIcon( bool is_open ) const
{
	UInteractMLTrainingSet* pexamples = Examples.Get();
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

	return FEditorStyle::GetBrush("ClassIcon.BoxComponent");
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
			return A->GetName().Compare( B->GetName() ) < 0;
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
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FTrainingSetEditor::OnRemoveItemClicked),
				FCanExecuteAction::CreateSP(this, &FTrainingSetEditor::CheckRemoveItemAllowed)),
			NAME_None,
			LOCTEXT("RemoveTrainingSetExampleText", "Remove"),
			LOCTEXT("RemoveTrainingSetExampleToolTip", "Remove the currently selected training set entry from the examples"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "DataTableEditor.Remove"));
	}
	ToolbarBuilder.EndSection();	
}

// regular updates
// NOTE: used to handle deferred details panel change
//
bool FTrainingSetEditor::Tick(float DeltaTime)
{
#if 0
	//deferred selection (by entry)
	if(DeferredSelectionEntry.IsValid())
	{
		//find and apply
		FTrainingSetTreeItem::Ptr found = FindHierarchyEntry( DeferredSelectionEntry.Get() );
		if(found.IsValid())
		{
			SetSelection( found );
		}

		//done
		DeferredSelectionEntry.Reset();
	}
	//deferred selection (by item)
	if(DeferredSelectionItem.IsValid())
	{
		SetSelection( DeferredSelectionItem );
		DeferredSelectionItem.Reset();
	}

	//sync details panel with our selected item
	const TArray<TWeakObjectPtr<UObject>>& cur_sel = DetailsView->GetSelectedObjects();
	int num_selected = cur_sel.Num();
	int num_wanted = (SelectedItem.IsValid() && SelectedItem->Resource.IsValid())?1:0;
	if(num_selected!=num_wanted 
		|| (num_selected>0 && cur_sel[0]!=SelectedItem->Resource)
		|| bForceApplySelection)
	{
		bForceApplySelection = false;

		//change selected item
		if(num_wanted>0)
		{
			DetailsView->SetObject( SelectedItem->Resource.Get() );
		}
		else
		{
			DetailsView->SetObject( nullptr );
		}
		DetailsView->ForceRefresh();

		//treeview too
		TreeView->SetSelection( SelectedItem );
	}
#endif

	return true; //keep on tickin
}


// can we currently remove an item?
bool FTrainingSetEditor::CheckRemoveItemAllowed() const
{
	return SelectedItem.IsValid(); //can only remove if user has selected something
//		&& SelectedItem!=ResourcesRoot;  //can never remove the roots
}


// remove the currently selected entry in the hierarchy
//
void FTrainingSetEditor::OnRemoveItemClicked()
{
#if 0
	FResourceListEditorTreeItem::Ptr current_item = SelectedItem;
	if(current_item)
	{
		RemoveEntry( current_item->Parent->AsShared(), current_item->IndexInParent );
	}
#endif	
}

// refresh internal representation of current resource list state
//
void FTrainingSetEditor::RebuildEntryViewModel( bool track_item_instead_of_entry )
{
	//should always only be root
	if(TreeRoot.Num()!=3)
	{
		TreeRoot.Empty();

//		ResourcesRoot = MakeShareable( new FTrainingSetTreeItem() );
//		TreeRoot.Add( ResourcesRoot );
	}

#if 0
	//store selection state
	if(SelectedItem.IsValid())
	{
		if(track_item_instead_of_entry)
		{
			DeferredSelect( SelectedItem );
		}
		else
		{
			TWeakObjectPtr<UInteractMLTrainingSet> selected_entry = SelectedItem.Get()->Resource;
			DeferredSelect( selected_entry );
		}
		bForceApplySelection = true; //likely re-applying same object, need force to update details and toolbar		
	}
	SetSelection( FTrainingSetTreeItem::Ptr() ); //clear actual selection for rebuild
	
	//incremental update
	UInteractMLTrainingSet* pres_list = GetEditableExamples();
	ResourcesRoot->Sync( pres_list->Resources );
	ReferencesRoot->Sync( pres_list->References );
	MissingRoot->SyncMissing( FInteractMLModule::GetModule()->GetMissingAssets(), ResourcesRoot->GetName() );
#endif
	RefreshTreeview();
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

// hierarchy UI creation
//
TSharedRef<SVerticalBox> FTrainingSetEditor::CreateTrainingSetHierarchyUI()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(TreeView, STrainingSetTreeView, SharedThis(this))
			.TreeItemsSource(&TreeRoot)
			.OnGenerateRow(this, &FTrainingSetEditor::GenerateTreeRow)
//			.OnItemScrolledIntoView(TreeView, &STrainingSetTreeView::OnTreeItemScrolledIntoView)
			.ItemHeight(21)
//			.SelectionMode(InArgs._SelectionMode)
//			.OnSelectionChanged(this, &FTrainingSetEditor::OnTreeViewSelect)
//			.OnExpansionChanged(this, &SPathView::TreeExpansionChanged)
			.OnGetChildren(this, &FTrainingSetEditor::GetChildrenForTree)
//			.OnSetExpansionRecursive(this, &SPathView::SetTreeItemExpansionRecursive)
//			.OnContextMenuOpening(this, &FTrainingSetEditor::GenerateTreeContextMenu)
			.ClearSelectionOnClick(false)
			.HighlightParentNodesForSelection(true)				
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Example() )
				.FillWidth(0.4f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetExampleColumnTitle","Example") )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Count() )
				.FillWidth(0.2f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetCountColumnTitle", "Count" ) )
					.Margin(4.0f)
				]
				+ SHeaderRow::Column( STrainingSetTreeView::ColumnTypes::Timestamp() )
				.FillWidth(0.4f)
				[
					SNew(STextBlock).Text( LOCTEXT("TrainingSetTimestampColumnTitle", "Timestamp" ) )
					.Margin(4.0f)
				]
			)
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
	SetSelection( Item );
}

// update our editor local selection state, show in Details tab
//
void FTrainingSetEditor::SetSelection( FTrainingSetTreeItem::Ptr Item )
{
	SelectedItem = Item;
}

// is user allowed to delete this item?
//
bool FTrainingSetEditor::CanDeleteItem( FTrainingSetTreeItem* item ) const
{
	//can delete any normal resource entry, not root though
	return false;
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
#if 0
		//delete/bs to remove entries	
		if ( InKeyEvent.GetKey() == EKeys::Delete || InKeyEvent.GetKey() == EKeys::BackSpace )
		{
			FTrainingSetTreeItem::Ptr item = training_set_editor.IsValid()?training_set_editor->SelectedItem:nullptr;
			if (item.IsValid())
			{
				if(training_set_editor->CanDeleteItem( item.Get() ))
				{
					//pass to editor to perform delete
					training_set_editor->RemoveEntry( item->Parent->AsShared(), item->IndexInParent );
				}
				
				return FReply::Handled();
			}
			
			return FReply::Handled();
		}
#endif
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

	CurrentHoverText = FText::FromString( Item->GetName() );
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
			UE_LOG(LogInteractML,Display,TEXT("DROPPED ITEM '%s' ONTO '%s'"), *psource->Item->GetName(), *pdestination_item->GetName() );
		}
	}
	return FReply::Handled();
}

// Row accessor handler: get name
//
FText STrainingSetTreeRow::GetRowName( FTrainingSetTreeItem::Ptr item ) const
{
	return FText::FromString( item->GetName() );
}

// Row accessor handler: get type
//
FText STrainingSetTreeRow::GetRowType( FTrainingSetTreeItem::Ptr item ) const
{
	return FText::FromString("Hello");
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
	TSharedPtr<FTrainingSetEditor> res_list_editor = TrainingSetEditorWeak.Pin();
	if(item->IsDimmed() && item!=res_list_editor->SelectedItem)	//(interferes with selection row highlight)
	{
		return FSlateColor::UseSubduedForeground(); //like in scene outliner
	}
	else
	{
		//TODO: work out why this causes icons to go black on selected rows
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
	// names and path
	if( ColumnName == STrainingSetTreeView::ColumnTypes::Example() )
	{
		TSharedPtr<FTrainingSetEditor> plisteditor = TrainingSetEditorWeak.Pin();

		//editable text (to support rename)
		TSharedPtr<SInlineEditableTextBlock> InlineTextBlock = SNew(SInlineEditableTextBlock)
			.Text(this, &STrainingSetTreeRow::GetRowName, ItemPtr )
//			.HighlightText( SceneOutliner.GetFilterHighlightText() )
			.ColorAndOpacity(this, &STrainingSetTreeRow::GetRowColourAndOpacity, ItemPtr);
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
						.ColorAndOpacity(this, &STrainingSetTreeRow::GetRowColourAndOpacity, ItemPtr )
					]
					+SHorizontalBox::Slot()
					.Padding(6,0,0,0)
					.VAlign(VAlign_Center)
					.FillWidth(1.0f)
					[ 
						InlineTextBlock.ToSharedRef()
//						SNew( STextBlock )
//						.Text( this, &STrainingSetTreeRow::GetRowName, ItemPtr )
//						.ColorAndOpacity(this, &STrainingSetTreeRow::GetRowColourAndOpacity, ItemPtr )
					]
				]
			];

			return ui;
	}
	//------------------------------------------------------------------------
	// type
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Count() )
	{
		// The second column displays the resource type
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( FText::FromString("42") )
				.ColorAndOpacity( FSlateColor::UseSubduedForeground() )	//like in scene outliner
			];
	}	
	//------------------------------------------------------------------------
	// asset information for this resource
	else if( ColumnName == STrainingSetTreeView::ColumnTypes::Timestamp())
	{
		// The second column displays the resource type
		return
			SNew( SHorizontalBox )
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew( STextBlock )
				.Text( FText::FromString("0:00:00") )
				.ColorAndOpacity( FSlateColor::UseSubduedForeground() )	//like in scene outliner
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
