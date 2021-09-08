//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLScripting.h"

//unreal
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

//module

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
void FInteractMLScriptingModule::StartupModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Starting InteractML Plugin - Scripting Module" ) );

}

// Exit
//
// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
// we call this function before unloading the module.
//
void FInteractMLScriptingModule::ShutdownModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Stopping InteractML Plugin - Scripting Module" ) );

}

// EPILOGUE
#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FInteractMLScriptingModule, InteractMLScripting)

