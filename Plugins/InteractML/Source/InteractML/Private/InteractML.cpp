//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractML.h"

//unreal
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

//module
#include "InteractMLStorage.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLModel.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"
DEFINE_LOG_CATEGORY(LogInteractML);

// CONSTANTS & MACROS

// default location of ML data in a project, overridable in game ini files
const TCHAR* c_DefaultDataDirectoryName = TEXT("../Data");


// LOCAL CLASSES & TYPES

//dev/unit testing (at end of file)
extern void InteractMLTests_Run();


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

	//testing during development
	//InteractMLTests_Run();
}

// Exit
//
// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
// we call this function before unloading the module.
//
void FInteractMLModule::ShutdownModule()
{
	UE_LOG( LogInteractML, Display, TEXT( "Stopping InteractML Plugin" ) );
	
	ShutdownCache();

	s_pModule = nullptr;
}


// resolve and store ML data storage root path
// To override, add the following to an InteractML section of the Config/DefaultGame.ini
//
//	[InteractML]
//	DataPath=my/custom/path/
//
// NOTE: this is relative to the Content folder of the project
// NOTE: The default is "../Data/", i.e. outside the content folder, but alongside
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
	DataRootPath = FPaths::Combine( FPaths::ProjectContentDir(), data_path );
	FPaths::NormalizeDirectoryName( DataRootPath );
	FPaths::CollapseRelativeDirectories( DataRootPath );
	UE_LOG( LogInteractML, Log, TEXT( "ML data file path set to: '%s'%s" ), *DataRootPath, is_overridden ? TEXT( "" ) : TEXT( " (default)" ) );

	//ensure exists
	if (DataRootPath.Len() > 0)
	{
		IPlatformFile& file_system = FPlatformFileManager::Get().GetPlatformFile();
		if(!file_system.DirectoryExists( *DataRootPath ))
		{
			file_system.CreateDirectoryTree( *DataRootPath );

			//verify
			if(!file_system.DirectoryExists( *DataRootPath ))
			{
				UE_LOG( LogInteractML, Error, TEXT( "Unable to create data storage directory: '%s'. Please check configuration, access permissions, and system status." ), *DataRootPath );
				DataRootPath = "";
			}
		}
	}
}

// release any tracked objects
//
void FInteractMLModule::ShutdownCache()
{
	//Doesn't seem to be needed at application shutdown, everything released anyway
	//(the removefromroot was crashing, presumably these objects are already removed earlier than the module on exit)
#if false
	//clear object lookup cache
	for (auto it = ObjectLookup.CreateIterator(); it; ++it)
	{
		UInteractMLStorage* pobj = it.Value().Get();
		if (pobj && pobj->bIsTemporary)
		{
			//done owning it, release to GC
			pobj->RemoveFromRoot();
		}
	}
	ObjectLookup.Empty();
#endif
}


///////////////////////////////// ML OBJECT MANAGEMENT ////////////////////////////////////


// ensure formatting is standardised and uniform
// NOTE: also removes case sensitivity
//
FString MakeFilePathKey(FString path)
{
	path = path.Replace(TEXT("\\"), TEXT("/"));
	path = path.ToUpper();
	return path;
}

// ml objects : obtain path based ones here to track and synchronise globally
//
UInteractMLTrainingSet* FInteractMLModule::GetTrainingSet( FString path_and_name )
{
	//clean and make key
	FString base_file_path = UInteractMLStorage::SanitisePathAndName( path_and_name );
	FString file_key = MakeFilePathKey( base_file_path ) + UInteractMLTrainingSet::cExtensionPrefix;
	
	//locate
	auto pentry = ObjectLookup.Find( file_key );
	UInteractMLStorage* pobj = pentry?(pentry->Get()):nullptr;
	
	//create on demand
	if(!pobj)
	{
		pobj = NewObject<UInteractMLTrainingSet>();

		//temp objects are owned by plugin
		pobj->AddToRoot();
		pobj->FInteracMLModule_SetBaseFilePath(base_file_path);	//will resolve/create/assign-id as needed, marks object as temp

		//store for lookup
		ObjectLookup.Add( file_key, pobj);
	}

	//check right type
	UInteractMLTrainingSet* ptraining_set = Cast<UInteractMLTrainingSet>(pobj);
	if (!ptraining_set)
	{
		UE_LOG(LogInteractML, Error, TEXT("Path '%s' doesn't refer to a training set"), *path_and_name);
	}

	return ptraining_set;
}

UInteractMLModel* FInteractMLModule::GetModel( UClass* model_type, FString path_and_name)
{
	//determine extension (need to query model type dynamically)
	FString model_file_extension = model_type->GetDefaultObject<UInteractMLModel>()->GetExtensionPrefix();

	//clean and make key
	FString base_file_path = UInteractMLStorage::SanitisePathAndName( path_and_name, model_file_extension );
	FString file_key = MakeFilePathKey(base_file_path) + model_file_extension;
	
	//locate
	auto pentry = ObjectLookup.Find( file_key );
	UInteractMLStorage* pobj = pentry?pentry->Get():nullptr;
	
	//create on demand
	if(!pobj)
	{
		pobj = NewObject<UInteractMLModel>( GetTransientPackage(), model_type, NAME_None);
		
		//temp objects are owned by plugin
		pobj->AddToRoot();
		pobj->FInteracMLModule_SetBaseFilePath(base_file_path);	//will resolve/create/assign-id as needed, marks object as temp

		//store for lookup
		ObjectLookup.Add( file_key, pobj);
	}
	
	//check right type
	UInteractMLModel* pmodel = Cast<UInteractMLModel>(pobj);
	if (!pmodel)
	{
		UE_LOG(LogInteractML, Error, TEXT("Failed to create model from file '%s'."), *path_and_name);
	}
	
	return pmodel;
}



// ml objects : inform of any obtained from direct asset references here as we need to synchronise with path based ones
//
void FInteractMLModule::SetTrainingSet(UInteractMLTrainingSet* training_set)
{
	//scan known objects for unsaved state
	for (auto It = ObjectLookup.CreateConstIterator(); It; ++It)
	{
		UInteractMLStorage* pstorage = It.Value().Get();
		if (pstorage == training_set)
		{
			//found, already registered
			return;
		}
	}
		
	//clean and make key
	FString file_key = MakeFilePathKey( training_set->GetBaseFilePath() ) + UInteractMLTrainingSet::cExtensionPrefix;
	
	//remember
	ObjectLookup.Add( file_key, training_set );
}

void FInteractMLModule::SetModel(UInteractMLModel* model)
{
	//scan known objects for unsaved state
	for (auto It = ObjectLookup.CreateConstIterator(); It; ++It)
	{
		UInteractMLStorage* pstorage = It.Value().Get();
		if (pstorage == model)
		{
			//found, already registered
			return;
		}
	}
	
	//clean and make key
	FString model_file_extension = model->GetExtensionPrefix();
	FString file_key = MakeFilePathKey( model->GetBaseFilePath() ) + model_file_extension;
	
	//remember
	ObjectLookup.Add( file_key, model );
}



///////////////////////////////// STATE ////////////////////////////////////

// save any unsaved training or model data, this is either called:
// 1. by editor module when world is saved (marked dirty when any changes found)
// 2. if you are a standalone app you will need to ensure this is called at some point after recording
//    There is a blueprint function for this InteractML -> Save
// NOTE: won't try and save any training sets or model that haven't changed
//
bool FInteractMLModule::Save()
{
	//scan known objects for unsaved state
	bool ok = true;
	for (auto It = ObjectLookup.CreateConstIterator(); It; ++It)
	{
		UInteractMLStorage* pstorage = It.Value().Get();
		if (pstorage && pstorage->HasUnsavedData())
		{
			//save it
			if (!pstorage->Save())
			{
				UE_LOG(LogInteractML, Error, TEXT("Failed to save InteractML object: %s"), *pstorage->GetFilePath());
				ok = false;
			}
		}
	}
	return ok;
}

// editor module needs to be able to tell if there is any new temp data objects?
// NOTE: different to assets, these are the temporary objects just wrapping external data
//
bool FInteractMLModule::HasUnsavedData() const
{
	//scan known objects for unsaved state
	for (auto It = ObjectLookup.CreateConstIterator(); It; ++It)
	{
		UInteractMLStorage* pstorage = It.Value().Get();
		if(pstorage && pstorage->HasUnsavedData() && pstorage->bIsTemporary)	//ARE temp
		{
			//found some unsaved state
			return true;
		}
	}

	//nothing found dirty
	return false;
}

// anything in unsaved assets?
//
bool FInteractMLModule::GetUnsavedAssets(TArray<UInteractMLStorage*>& changed_assets)
{
	bool found_any = false;
	//scan known objects for unsaved state
	for (auto It = ObjectLookup.CreateConstIterator(); It; ++It)
	{
		UInteractMLStorage* pstorage = It.Value().Get();
		if(pstorage && pstorage->HasUnsavedData() && !pstorage->bIsTemporary)	//NOT temp
		{
			//found some unsaved state
			changed_assets.Add(pstorage);
			found_any = true;
		}
	}
	
	return found_any;
}




// EPILOGUE
#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FInteractMLModule, InteractML)

