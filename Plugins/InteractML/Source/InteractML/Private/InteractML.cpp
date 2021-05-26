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
#define LOCTEXT_NAMESPACE "InteractML"
DEFINE_LOG_CATEGORY(LogInteractML);

// CONSTANTS & MACROS

// default location of ML data in a project, overridable in game ini files
//
const TCHAR* c_DefaultDataDirectoryName = TEXT("Data");


// LOCAL CLASSES & TYPES

// GLOBAL STATE
FInteractMLModule* FInteractMLModule::s_pModule = nullptr;

// CLASS STATE

// Init
//
// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//
void FInteractMLModule::StartupModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Starting InteractML Plugin" ) );
	s_pModule = this;

	InitPaths();
}


// Exit
//
// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
// we call this function before unloading the module.
//
void FInteractMLModule::ShutdownModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Stopping InteractML Plugin" ) );
	
	
	s_pModule = nullptr;
}


// resolve and store ML data storage root path
// To override, add the following to an InteractML section of the Config/DefaultGame.ini
//
//	[InteractML]
//	DataPath=my/custom/path/
//
// NOTE: this is relative to the Content folder of the project
// NOTE: The default is "Data/"
//
void FInteractMLModule::InitPaths()
{
	//look for data path specified in game ini
	FString data_path;
	bool is_overridden = false;
	if(GConfig && GConfig->GetString(TEXT("InteractML"), TEXT("DataPath"), data_path, GGameIni))
	{
		is_overridden = true;
	}

	//fallback to default if not found
	if(!is_overridden || data_path.Len()==0)
	{
		data_path = c_DefaultDataDirectoryName;
	}

	//here, use this
	m_DataRootPath = FPaths::Combine( FPaths::ProjectContentDir(), data_path );
	UE_LOG( LogInteractML, Log, TEXT( "ML data file path set to: '%s'%s" ), *m_DataRootPath, is_overridden ? TEXT( "" ) : TEXT( " (default)" ) );

	//ensure exists
	if (m_DataRootPath.Len() > 0)
	{
		IPlatformFile& file_system = FPlatformFileManager::Get().GetPlatformFile();
		if(!file_system.DirectoryExists( *m_DataRootPath ))
		{
			file_system.CreateDirectoryTree( *m_DataRootPath );

			//verify
			if(!file_system.DirectoryExists( *m_DataRootPath ))
			{
				UE_LOG( LogInteractML, Error, TEXT( "Unable to create data storage directory: '%s'. Please check configuration, access permissions, and system status." ), *m_DataRootPath );
				m_DataRootPath = "";
			}
		}
	}
}




// EPILOGUE
#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FInteractMLModule, InteractML)

