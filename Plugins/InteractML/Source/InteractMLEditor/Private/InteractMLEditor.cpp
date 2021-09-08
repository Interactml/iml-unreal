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

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"
DEFINE_LOG_CATEGORY(LogInteractML);

// CONSTANTS & MACROS

// LOCAL CLASSES & TYPES

// GLOBAL STATE

// CLASS STATE

// Init
//
// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//
void FInteractMLEditorModule::StartupModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Starting InteractML Plugin - Editor Module" ) );

	InitHooks();
	InitAssets();
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
	
	//style setup for asset appearance
	AssetStyleSet = MakeShareable(new FSlateStyleSet("InteractML"));
	AssetStyleSet->SetContentRoot(ContentDir);
	FSlateImageBrush* TrainingSetThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLTrainingSet"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (TrainingSetThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLTrainingSet", TrainingSetThumbnail);
	}
	FSlateImageBrush* ClassificationThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLClassification"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (ClassificationThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLClassificationModel", ClassificationThumbnail);
	}
	FSlateImageBrush* RegressionThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLRegression"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (RegressionThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLRegressionModel", RegressionThumbnail);
	}
	FSlateImageBrush* DTWThumbnail = new FSlateImageBrush(AssetStyleSet->RootToContentDir( TEXT("Icons/InteractMLDynamicTimewarp"), TEXT(".png") ), FVector2D(128.f, 128.f) );
	if (DTWThumbnail)
	{
		AssetStyleSet->Set("ClassThumbnail.InteractMLDynamicTimewarpModel", DTWThumbnail);
	}
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

