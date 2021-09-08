//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLEditor.h"

//unreal
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Styling/SlateStyleRegistry.h"

//module
#include "InteractML.h"
#include "InteractMLTrainingSetActions.h"
#include "InteractMLModelActions.h"
#include "InteractMLStorage.h"
#include "InteractMLLabelActions.h"
#include "InteractMLLabelTableActions.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"
DEFINE_LOG_CATEGORY(LogInteractML);

// CONSTANTS & MACROS

// LOCAL CLASSES & TYPES

// GLOBAL STATE
class FInteractMLEditorModule* FInteractMLEditorModule::EditorModule = nullptr;

// CLASS STATE

// Init
//
// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//
void FInteractMLEditorModule::StartupModule()
{
	EditorModule = this;

	UE_LOG( LogInteractML, Display, TEXT( "Starting InteractML Plugin - Editor Module" ) );

	InitHooks();
	InitAssets();
	InitExtensibility();
}

// Exit
//
// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
// we call this function before unloading the module.
//
void FInteractMLEditorModule::ShutdownModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Stopping InteractML Plugin - Editor Module" ) );

	ShutdownHooks();
	ShutdownAssets();
	ShutdownExtensibility();
}

// start monitoring global editor events we are interested in
//
void FInteractMLEditorModule::InitHooks()
{
	FEditorDelegates::EndPIE.AddRaw(this, &FInteractMLEditorModule::OnEndPIE);
	FEditorDelegates::PostSaveWorld.AddRaw(this, &FInteractMLEditorModule::OnWorldSaved);
}

// stop monitoring global editor events we were interested in
//
void FInteractMLEditorModule::ShutdownHooks()
{
	FEditorDelegates::EndPIE.RemoveAll(this);
	FEditorDelegates::PostSaveWorld.RemoveAll(this);
}

// register asset types with the editor
//
void FInteractMLEditorModule::InitAssets()
{
	// gather
	FString ContentDir = IPluginManager::Get().FindPlugin("InteractML")->GetBaseDir() / "Content";
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	//use custom category for interact ML
	EAssetTypeCategories::Type interactml_category = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("InteractML")), LOCTEXT("InteractMLAssetCategory", "InteractML"));
	
	//register our asset types
	AssetTools.RegisterAssetTypeActions( MakeShareable( new FInteractMLTrainingSetActions(interactml_category) ));
	AssetTools.RegisterAssetTypeActions( MakeShareable( new FInteractMLClassificationModelActions(interactml_category) ));
	AssetTools.RegisterAssetTypeActions( MakeShareable( new FInteractMLRegressionModelActions(interactml_category) ));
	AssetTools.RegisterAssetTypeActions( MakeShareable( new FInteractMLDynamicTimewarpModelActions(interactml_category) ));
	AssetTools.RegisterAssetTypeActions( MakeShareable( new FInteractMLLabelActions(interactml_category) ));
	AssetTools.RegisterAssetTypeActions( MakeShareable( new FInteractMLLabelTableActions(interactml_category) ));
	
	//style setup for icons
	AssetStyleSet = MakeShareable(new FSlateStyleSet("InteractML"));
	AssetStyleSet->SetContentRoot(ContentDir);
	
	//asset icons
	FSlateImageBrush* TrainingSetThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLTrainingSet_128"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (TrainingSetThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLTrainingSet", TrainingSetThumbnail);
	}
	FSlateImageBrush* ClassificationThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLClassification_128"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (ClassificationThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLClassificationModel", ClassificationThumbnail);
	}
	FSlateImageBrush* RegressionThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLRegression_128"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (RegressionThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLRegressionModel", RegressionThumbnail);
	}
	FSlateImageBrush* DTWThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLDynamicTimewarp_128"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (DTWThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLDynamicTimewarpModel", DTWThumbnail);
	}
	FSlateImageBrush* LabelThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLLabel_128"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (LabelThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLLabel", LabelThumbnail);
	}
	FSlateImageBrush* LabelTableThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLLabelTable_128"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (LabelTableThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLLabelTable", LabelTableThumbnail);
	}

	//node icons
	FSlateImageBrush* NodeIcon = new FSlateImageBrush( AssetStyleSet->RootToContentDir( TEXT( "Icons/InteractMLNode_16" ), TEXT( ".png" ) ), FVector2D( 16.f, 16.f ) );
	if(NodeIcon)
	{
		AssetStyleSet->Set( "NodeIcons.Default_16x", NodeIcon );
	}

	//treeview icons
	FSlateImageBrush* TreeIcon_SingleExample = new FSlateImageBrush( AssetStyleSet->RootToContentDir( TEXT( "Icons/InteractMLExampleSingle_16" ), TEXT( ".png" ) ), FVector2D( 16.f, 16.f ) );
	if(TreeIcon_SingleExample)
	{
		AssetStyleSet->Set( "TreeViewIcons.SingleExample_16x", TreeIcon_SingleExample );
	}

	//misc icons
	FSlateImageBrush* MiscIcon_TrainingSet_Small = new FSlateImageBrush( AssetStyleSet->RootToContentDir( TEXT( "Icons/InteractMLTrainingSet_16" ), TEXT( ".png" ) ), FVector2D( 16.f, 16.f ) );
	if(MiscIcon_TrainingSet_Small)
	{
		AssetStyleSet->Set( "MiscIcons.TrainingSet_16x", MiscIcon_TrainingSet_Small );
	}

	//toolbar icons
	FSlateImageBrush* ToolbarIcon_Delete = new FSlateImageBrush( AssetStyleSet->RootToContentDir( TEXT( "Icons/InteractMLDelete_40" ), TEXT( ".png" ) ), FVector2D( 40.f, 40.f ) );
	if(ToolbarIcon_Delete)
	{
		AssetStyleSet->Set( "ToolbarIcons.Delete_40x", ToolbarIcon_Delete );
	}
	FSlateImageBrush* ToolbarIcon_Reset = new FSlateImageBrush( AssetStyleSet->RootToContentDir( TEXT( "Icons/InteractMLReset_40" ), TEXT( ".png" ) ), FVector2D( 40.f, 40.f ) );
	if(ToolbarIcon_Reset)
	{
		AssetStyleSet->Set( "ToolbarIcons.Reset_40x", ToolbarIcon_Reset );
	}
	
	//register for use
	FSlateStyleRegistry::RegisterSlateStyle(*AssetStyleSet);
}

// deregister asset types
//
void FInteractMLEditorModule::ShutdownAssets()
{
	//release our custom style
	FSlateStyleRegistry::UnRegisterSlateStyle( *AssetStyleSet.Get() );
	ensure(AssetStyleSet.IsUnique());
	AssetStyleSet.Reset();
}

// setup extensibility systems for extending editor and building custom panels
//
void FInteractMLEditorModule::InitExtensibility()
{
	//MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
	
}

// clean up
//
void FInteractMLEditorModule::ShutdownExtensibility()
{
	//MenuExtensibilityManager.Reset();
	ToolBarExtensibilityManager.Reset();
}

// play in editor has stopped.  an opportunity to propertly flag any unsaved state as dirty
//
void FInteractMLEditorModule::OnEndPIE(const bool bIsSimulating)
{
	//check with runtime if anything new, then mark world dirty
	if (FInteractMLModule::Get().HasUnsavedData())
	{
		UE_LOG(LogInteractML, Log, TEXT("New data detected during PiE session, marking world dirty because save is required to keep changes."));
		
		//flag something as dirty to ensure save is triggered by user (see: OnWorldSaved below)
		if(GEditor->EditorWorld)
		{
			GEditor->EditorWorld->MarkPackageDirty();
		}
	}

	//also any assets modified during PIE need marking
	TArray<UInteractMLStorage*> changed_assets;
	if(FInteractMLModule::Get().GetUnsavedAssets(changed_assets))
	{
		for (int i = 0; i < changed_assets.Num(); i++)
		{
			changed_assets[i]->MarkPackageDirty();
		}
	}
}

// a 'global' save has happened.  a chance to save our unsaved training/model state
//
void FInteractMLEditorModule::OnWorldSaved(uint32 SaveFlags, UWorld* World, bool bSuccess)
{
	//request runtime save any unsaved state
	FInteractMLModule::Get().Save();	
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FInteractMLEditorModule, InteractMLEditor)

