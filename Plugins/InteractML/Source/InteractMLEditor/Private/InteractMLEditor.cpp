//----
// InteractML - University Of Arts London
//----

#include "InteractMLEditor.h"

//unreal
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

//module
#include "InteractML.h"

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

