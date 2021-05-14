//----
// InteractML - University Of Arts London
//----

#include "InteractML.h"

//unreal
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

//module

// PROLOGUE
#define LOCTEXT_NAMESPACE "FInteractMLModule"
DEFINE_LOG_CATEGORY(LogInteractML);

// CONSTANTS & MACROS

// LOCAL CLASSES & TYPES

// GLOBAL STATE

// CLASS STATE

// Init
//
// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//
void FInteractMLModule::StartupModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Starting InteractML Plugin" ) );

}


// Exit
//
// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
// we call this function before unloading the module.
//
void FInteractMLModule::ShutdownModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Stopping InteractML Plugin" ) );

}

// EPILOGUE
#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FInteractMLModule, InteractML)

