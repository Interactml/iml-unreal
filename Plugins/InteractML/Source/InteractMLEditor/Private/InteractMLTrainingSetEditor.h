//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "IDetailCustomization.h"
#include "DragAndDrop/DecoratedDragDropOp.h"
#include "SDropTarget.h"

//module


//types of property to show
enum class EInteractMLTrainingSetProperty
{
	ExampleCount,
	SampleMode,
	LabelCount,
	ParameterCount,
	LabelType
};


// view model for a training set entry
//
struct FTrainingSetTreeItem : public TSharedFromThis<FTrainingSetTreeItem>
{
	//types
	typedef TSharedPtr<FTrainingSetTreeItem> Ptr;
	typedef TSharedRef<FTrainingSetTreeItem> Ref;
	
	//the model we are the view model for
	TWeakObjectPtr<class UInteractMLTrainingSet> Examples;

	//persistent/unique ID of the example we represent
	int ExampleID;

	//specific example in the training set, the label and example we represent
	int ExampleIndex;

	//parent item in hierarchy
	FTrainingSetTreeItem* Parent;	//(not TSharedPtr as existence is implied, don't want loops)
	int IndexInParent;

	//hierarchy to match the models hierarchy
	TArray< FTrainingSetTreeItem::Ptr > Children;

	//init
	FTrainingSetTreeItem()
		: Examples( nullptr )
		, ExampleID(0)
		, ExampleIndex(0)
		, Parent( nullptr )
		, IndexInParent( 0 )
	{}
	FTrainingSetTreeItem(UInteractMLTrainingSet* pexamples, int example_index, FTrainingSetTreeItem::Ptr parent = nullptr, int index_in_parent = 0);

	//access
	int GetExampleID() const { return ExampleID; }
	FString GetLabel() const;
	bool IsDimmed() const { return false; }
	const FSlateBrush* GetIcon(bool is_open) const;
	int GetSamples() const;
	FText GetDurationText() const;
	FText GetSessionText() const;
	FText GetUser() const;

	//editing
	UObject* GetExamples();
	void Sync(class UInteractMLTrainingSet* proot );
	void Sync(class UInteractMLTrainingSet* pentry, FTrainingSetTreeItem::Ptr parent, int index_in_parent );
};

	
/// <summary>
/// Fully custom editing panel for training sets
/// </summary>
class FTrainingSetEditor
	: public FAssetEditorToolkit
	, public FEditorUndoClient
{
	/** TrainingSet Editor app identifier string */
	static const FName TrainingSetEditorAppIdentifier;
	
	/**	The tab id for the trainingset hierarchy tab */
	static const FName TrainingSetHierarchyTabId;

	/**	The tab id for the resource properties tab */
//	static const FName TrainingSetDetailsTabId;
	
	/** The workspace menu category of this toolkit */
	TSharedPtr<FWorkspaceItem> WorkspaceMenuCategory;
	
	/** Property viewing widget */
//	TSharedPtr<class IDetailsView> DetailsView;

	/** UI for the "Training Set Hierarchy" tab */
	TSharedPtr<SWidget> TrainingSetHierarchyTabWidget;
	
	// UI for treeview
	TSharedPtr< STreeView< TSharedPtr<FTrainingSetTreeItem>> > TreeView;

	// roots of view model
	TArray<FTrainingSetTreeItem::Ptr> TreeRoots;
	
	// currently selected item
	TArray<FTrainingSetTreeItem::Ptr> SelectedItems;

	// tick management
	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;

	//tracking
	int PrevExampleCount;

	// deferred selection (by entry or item)
	//TWeakObjectPtr<UInteractMLTrainingSet> DeferredSelectionEntry;
	//FTrainingSetTreeItem::Ptr DeferredSelectionItem;
	//bool bForceApplySelection;
	
public:
	//setup
	FTrainingSetEditor();
	virtual ~FTrainingSetEditor();
	void InitTrainingSetEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UInteractMLTrainingSet* examples );

	//access

	/** FAssetEditorToolkit interface */
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	
	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	/** FEditorUndoClient */
	virtual void PostUndo( bool bSuccess ) override;
	virtual void PostRedo( bool bSuccess ) override;

	//utility
	
private:

	//access
	class UInteractMLTrainingSet* GetEditableExamples() const;
//	FTrainingSetTreeItem::Ptr FindHierarchyEntry( UObject* entry_or_asset );
	
	//ui setup
	void CreateAndRegisterTrainingSetHierarchyTab(const TSharedRef<class FTabManager>& InTabManager);
	TSharedRef<SDockTab> SpawnTab_TrainingSetHierarchy( const FSpawnTabArgs& Args );
	TSharedRef<SVerticalBox> CreateTrainingSetHierarchyUI();
	void ExtendToolbar(TSharedPtr<FExtender> Extender);	
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	TSharedRef<SWidget> CreateInfoField(FText name, UInteractMLTrainingSet* pexamples, EInteractMLTrainingSetProperty property_type, FText tooltip, float max_text_width=75.0f);
	FText GetPropertyText( UInteractMLTrainingSet* pexamples, EInteractMLTrainingSetProperty property_type ) const;

	//idle/deferred updates
	bool Tick(float DeltaTime);
	void TrackChanges();
	
	//editing via ui
	bool CheckDeleteAllowed() const;
	void OnDeleteClicked();
	bool CheckResetAllowed() const;
	void OnResetClicked();
	//void ShowAssetTypeContextMenu();
	//edit handlers
	bool DeleteSelectedExamples();
	bool CanDeleteItem( FTrainingSetTreeItem* item ) const;
	bool IsItemValidDragSource( FTrainingSetTreeItem* item ) const;

	//editing data
	void OnUndoRedo();
	
	//treeview
	TSharedRef<ITableRow> GenerateTreeRow( TSharedPtr<FTrainingSetTreeItem> TreeItem, const TSharedRef<STableViewBase>& OwnerTable );
	void GetChildrenForTree( TSharedPtr< FTrainingSetTreeItem > TreeItem, TArray< TSharedPtr<FTrainingSetTreeItem> >& OutChildren );
	void RebuildEntryViewModel( bool select_latest_example=false );
	void RefreshTreeview(); //update Slate-side when our viewmodel has chnaged
	FTrainingSetTreeItem::Ptr BuildEntryViewModel( UInteractMLTrainingSet* pentry );
	void OnTreeViewSelect( FTrainingSetTreeItem::Ptr Item, ESelectInfo::Type SelectInfo );
	FTrainingSetTreeItem::Ptr FindTreeItemByExampleID( int example_id );
	static FTrainingSetTreeItem::Ptr FindTreeItemByExampleID( FTrainingSetTreeItem::Ptr, int example_id );
	FTrainingSetTreeItem::Ptr FindTreeItemByIndex( int example_index );
	static FTrainingSetTreeItem::Ptr FindTreeItemByIndex( FTrainingSetTreeItem::Ptr, int example_index, int& current_index );
		
	//notifications
	void OnHierarchyTabActivated(TSharedRef<SDockTab> tab, ETabActivationCause cause);

	friend class STrainingSetTreeView;
	friend class STrainingSetTreeRow;
};


//------------------------------------------------------------------------
// tree view
//
class STrainingSetTreeView : public STreeView< TSharedPtr<FTrainingSetTreeItem> >
{
	//FTrainingSetTreeItem::Ptr PendingRenameItem;

protected:
	
	/** Weak reference to the outliner widget that owns this list */
	TWeakPtr<FTrainingSetEditor> TrainingSetEditorWeak;
	
public:
	//static info
	struct ColumnTypes
	{	
		static const FName& Group()
		{
			static FName Col("Group");
			return Col;
		}		
		static const FName& Number()
		{
			static FName Col("Number");
			return Col;
		}	
		static const FName& Label()
		{
			static FName Col("Label");
			return Col;
		}
		static const FName& Samples()
		{
			static FName Col("Samples");
			return Col;
		}
		static const FName& Duration()
		{
			static FName Col("Duration");
			return Col;
		}
		static const FName& Session()
		{
			static FName Col("Session");
			return Col;
		}
		static const FName& User()
		{
			static FName Col("User");
			return Col;
		}
		static const FName& Notes()
		{
			static FName Col("Notes");
			return Col;
		}
	};

public:
	
	/** Construct this widget */
	void Construct(const FArguments& InArgs, TSharedRef<FTrainingSetEditor> Owner);	

	/** STreeView */
	virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override;
		
	//access
	TWeakPtr<FTrainingSetEditor> GetTrainingSetEditor() { return TrainingSetEditorWeak; }

};


//------------------------------------------------------------------------
// drag and drop payload - not used yet
//
class FTrainingSetDragDrop : public FDecoratedDragDropOp
{
public:
	FTrainingSetTreeItem::Ptr Item;

public:
	DRAG_DROP_OPERATOR_TYPE(FTrainingSetListDragDrop, FDragDropOperation)	
	
	FTrainingSetDragDrop( FTrainingSetTreeItem::Ptr dragged_item );
	virtual void Construct() override;
	virtual ~FTrainingSetDragDrop() {}
		
	// FDragDropOperation interface
#if 0
	virtual void OnDrop( bool bDropWasHandled, const FPointerEvent& MouseEvent ) override { Impl->OnDrop(bDropWasHandled, MouseEvent); }
	virtual void OnDragged( const class FDragDropEvent& DragDropEvent ) override { Impl->OnDragged(DragDropEvent); }
	virtual FCursorReply OnCursorQuery() override { return Impl->OnCursorQuery(); }
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override { return Impl->GetDefaultDecorator(); }
	virtual FVector2D GetDecoratorPosition() const override { return Impl->GetDecoratorPosition(); }
	virtual void SetDecoratorVisibility(bool bVisible) override { Impl->SetDecoratorVisibility(bVisible); }
	virtual bool IsExternalOperation() const override { return Impl->IsExternalOperation(); }
	virtual bool IsWindowlessOperation() const override { return Impl->IsWindowlessOperation(); }
#endif
	// End of FDragDropOperation interface
		
private:
	//TSharedPtr<FTrainingSetDragDrop> Impl;
};


/**
* A widget that displays a hover cue and handles dropping entries of allowed types onto this widget
*/
class STrainingSetItemDropTarget : public SDropTarget
{
public:
	/** Called when a valid asset is dropped */
	DECLARE_DELEGATE_OneParam( FOnAssetDropped, UObject* );
	
	/** Called when we need to check if an asset type is valid for dropping */
	DECLARE_DELEGATE_RetVal_OneParam( bool, FIsAssetAcceptableForDrop, const UObject* );

	/** Called when a valid item is dropped */
	DECLARE_DELEGATE_OneParam( FOnItemDropped, FTrainingSetTreeItem::Ptr );
	
	/** Called when we need to check if an asset type is valid for dropping */
	DECLARE_DELEGATE_RetVal_OneParam( bool, FIsItemAcceptableForDrop, FTrainingSetTreeItem::Ptr );
	
	SLATE_BEGIN_ARGS(STrainingSetItemDropTarget)
	{ }
	/* Content to display for the in the drop target */
	SLATE_DEFAULT_SLOT( FArguments, Content )
	/** Called when a valid asset is dropped */
	SLATE_EVENT( FOnAssetDropped, OnAssetDropped )
	/** Called to check if an asset is acceptible for dropping */
	SLATE_EVENT( FIsAssetAcceptableForDrop, OnIsAssetAcceptableForDrop )
	/** Called when a valid item is dropped */
	SLATE_EVENT( FOnItemDropped, OnItemDropped )
	/** Called to check if an item is acceptible for dropping */
	SLATE_EVENT( FIsItemAcceptableForDrop, OnIsItemAcceptableForDrop )
	SLATE_END_ARGS()
		
	void Construct(const FArguments& InArgs );
					
protected:
	FReply OnDropped(TSharedPtr<FDragDropOperation> DragDropOperation);
	virtual bool OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation) const override;
	virtual bool OnIsRecognized(TSharedPtr<FDragDropOperation> DragDropOperation) const override;
	
private:
	UObject* GetDroppedObject(TSharedPtr<FDragDropOperation> DragDropOperation, bool& bOutRecognizedEvent) const;
	FTrainingSetTreeItem::Ptr GetDroppedItem(TSharedPtr<FDragDropOperation> DragDropOperation, bool& bOutRecognizedEvent) const;
	
private:
	/** Delegate to call when an asset is dropped */
	FOnAssetDropped OnAssetDropped;
	/** Delegate to call to check validity of the asset */
	FIsAssetAcceptableForDrop OnIsAssetAcceptableForDrop;
	/** Delegate to call when an item is dropped */
	FOnItemDropped OnItemDropped;
	/** Delegate to call to check validity of the item */
	FIsItemAcceptableForDrop OnIsItemAcceptableForDrop;
};


//------------------------------------------------------------------------
// tree view row
//
/** Widget that represents a row in the outliner's tree control.  Generates widgets for each column on demand. */
class STrainingSetTreeRow
	: public SMultiColumnTableRow< TSharedPtr<FTrainingSetTreeItem> >
{
	
public:
	
	SLATE_BEGIN_ARGS( STrainingSetTreeRow ) {}
	
	/** The list item for this row */
	SLATE_ARGUMENT( FTrainingSetTreeItem::Ptr, Item )

	/** The owning object. This allows us access to the actual data table being edited as well as some other API functions. */
	SLATE_ARGUMENT(TSharedPtr<FTrainingSetEditor>, TrainingSetEditor)
			
	SLATE_END_ARGS()
			
			
	/** Construct function for this widget */
	void Construct( const FArguments& InArgs, const TSharedRef<STableViewBase>& treeview );
	
	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the tree row. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn( const FName& ColumnName ) override;
			
public:
	
protected:
	
	/** SWidget interface */
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;	
	//drag handlers
	virtual FReply OnMouseButtonDown( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent ) override;
	FReply OnDragDetected( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent );
		
	//drop handlers
	bool OnAssetDraggedOverName( const UObject* asset );
	void OnAssetDroppedName( UObject* asset );
	bool OnItemDraggedOverName( FTrainingSetTreeItem::Ptr item );
	void OnItemDroppedName( FTrainingSetTreeItem::Ptr item );
	bool OnAssetDraggedOverSlot( const UObject* asset );
	void OnAssetDroppedSlot( UObject* asset );
	
private:
	
	/** Weak reference to the outliner widget that owns our list */
	TWeakPtr< FTrainingSetEditor > TrainingSetEditorWeak;
	
	/** The item associated with this row of data */
	TWeakPtr<FTrainingSetTreeItem> Item;
	
	//test
	FText GetRowName( FTrainingSetTreeItem::Ptr item ) const;
	const FSlateBrush* GetRowIcon( FTrainingSetTreeItem::Ptr item ) const;
	FSlateColor GetRowColourAndOpacity( FTrainingSetTreeItem::Ptr item ) const;
	
protected:
	
private:
	
};

