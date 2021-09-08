//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLStorage.h"

//unreal
#include "Misc/EngineVersionComparison.h"
#include "Misc/FileHelper.h"

//module
#include "InteractML.h"
#include "InteractMLTrainingSet.h"
#include "InteractMLModel.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// json format file extension
FString UInteractMLStorage::cFileFormatExtension( TEXT(".json") );


// LOCAL CLASSES & TYPES


// various setup happens when the base_file_path is set
// 1. search for file data file with this base name (filters on expected extension/type)
// 2. see if the file has an ID, if so, adopt it
// 4. unless an existing file with no ID, one is extracted or generated
// 5. if no file, a file is created based on base path/name, id, and type
// 6. if an existing file was found then it's state is loaded into the ML object
//
void UInteractMLStorage::FInteracMLModule_SetBaseFilePath( FString base_file_path )
{
	//this is set up explicitly when created for a node that is using file path based data file referencing instead of assets
	//NOTE: the object is temporary, the data file is not
	bIsTemporary = true;

	//gather
	IPlatformFile& file_system = FPlatformFileManager::Get().GetPlatformFile();
	FString root_path = FInteractMLModule::Get().GetDataRoot();
	FString full_extension = GetExtensionPrefix() + cFileFormatExtension;

	//store
	BaseFilePath = base_file_path;

	//find
	bool found_file = false;
	FString full_file_path;
	TArray<FString> all_data_files;
	FString model_file_extension = GetExtensionPrefix();
	file_system.FindFilesRecursively( all_data_files, *root_path, *full_extension );
	for (int i = 0; i < all_data_files.Num(); i++)
	{
		//compare base path/name only for match
		FString file_path = all_data_files[i];		
		FString just_path_and_name = SanitisePathAndName(file_path, model_file_extension );
		if (just_path_and_name.Compare(base_file_path, ESearchCase::IgnoreCase)==0)
		{
			//yes, we found our file
			found_file = true;
			full_file_path = file_path;
			break;
		}
	}

	//where is ID coming from?
	if (found_file)
	{
		//use id in file (if present)
		//NOTE: ML objects CAN be based around files without IDs, they just never set one up
		//NOTE: All NEW objects created by the plugin are assigned one
		FileID = ExtractGuidFromFile( full_file_path );
	}
	else
	{
		//need a new one
		FileID = FGuid::NewGuid();
	}

	//load/create a file?
	if (found_file)
	{
		//use file data as initial state
		Load();
	}
	else
	{
		//initial file is only written on first save, this means empty/temp objects aren't persisted
		//we do need to offer it the chance to set it self up though if we're not loading
		Create();
	}
}

// is the base path being used for storage?
//
bool UInteractMLStorage::CheckBasePath(FString base_file_path) const
{
	FString model_file_extension = GetExtensionPrefix();
	base_file_path = SanitisePathAndName( base_file_path, model_file_extension );
	return BaseFilePath.Compare( base_file_path, ESearchCase::IgnoreCase ) == 0;
}

// build full directory to location of the data file
// e.g. D:/MLProject/Content/Examples/
//
FString UInteractMLStorage::GetDirectoryPath() const
{
	//root
	FString dir_root = FInteractMLModule::Get().GetDataRoot();
	
	//base path (if there is one)
	FString dir_part = GetBaseFilePathDirectoryPart();
	
	return FPaths::Combine(dir_root, dir_part);
}

// build full file path for storage
// e.g. D:/MLProject/Content/Examples/HandGestures.19D38579C13759B1.training.json, although pure file-based ones may not have an ID
//
FString UInteractMLStorage::GetFilePath() const
{
	FString path;

	//root
	path = FInteractMLModule::Get().GetDataRoot();

	//base file
	check(BaseFilePath.Len() > 0);
	path = FPaths::Combine(path, BaseFilePath);

	//ID (if has one)
	if (FileID.IsValid())
	{
		path += ".";
		path += FileID.ToString(EGuidFormats::Digits);
	}

	//type prefix
	path += GetExtensionPrefix();

	//file format extension
	path += cFileFormatExtension;

	return path;
}

// any sub-dir part of base file path
// e.g. "Examples/Foo/HandGestures" -> "Examples/Foo"
// e.g. "Hello" -> ""
//
FString UInteractMLStorage::GetBaseFilePathDirectoryPart() const
{
	//last separator decides split between parts
	int ifs = -1, ibs = -1;
	BaseFilePath.FindLastChar(TCHAR('/'), ifs);
	BaseFilePath.FindLastChar(TCHAR('\\'), ibs);
	int isep = FMath::Max(ifs, ibs);

	//want part before split
	if (isep != -1)
	{
		return BaseFilePath.Left(isep);
	}
	return "";
}

// the name part of base file path
// e.g. "Examples/Foo/HandGestures" -> "HandGestures"
// e.g. "Hello" -> "Hello"
//
FString UInteractMLStorage::GetBaseFilePathNamePart() const
{
	//last separator decides split between parts
	int ifs=-1, ibs=-1;
	BaseFilePath.FindLastChar(TCHAR('/'), ifs);
	BaseFilePath.FindLastChar(TCHAR('\\'), ibs);
	int iname = FMath::Max(ifs+1, ibs+1);
	
	//want part after split
	return BaseFilePath.RightChop( iname );
}


// save this ML objects internal state to disk
//
bool UInteractMLStorage::Load()
{
	//gather
	FString path = GetFilePath();
	IPlatformFile& file_system = FPlatformFileManager::Get().GetPlatformFile();
	
	//file exists?
	if (!file_system.FileExists(*path))
	{
		//no file, empty json
		FString none;
		return LoadJson( none );
	}

	//have file, load that
	FString json_string;
	if(FFileHelper::LoadFileToString( json_string, *path ))
	{
		return LoadJson( json_string );
	}		

	//failed
	return false;
}

// save this ML objects internal state to disk
//
bool UInteractMLStorage::Save() const
{
	//gather
	FString path = GetFilePath();
	IPlatformFile& file_system = FPlatformFileManager::Get().GetPlatformFile();
	
	//ensure directory
	FString directory = GetDirectoryPath();
	if(!file_system.DirectoryExists( *directory ))
	{
		file_system.CreateDirectoryTree( *directory );
	}
	if (!file_system.DirectoryExists(*directory))
	{
		return false;
	}

	//generate Json
	FString json_string; 
	SaveJson( json_string );
	
	//write file
#if UE_VERSION_OLDER_THAN(4,26,0)   //API change in SaveStringToFile
	if(FFileHelper::SaveStringToFile( json_string, *path ))
#else
	FStringView whole_string( json_string );
	if(FFileHelper::SaveStringToFile( whole_string, *path ))
#endif
	{
		//nolonger needs save
		bNeedsSave = false;
		return true;
	}
	
	//failed
	return false;
}

// data has been modified
// NOTE: this is a separate system to Unreals own dirty/save system since our model/example state isn't persisted
//   in the object normally, instead we store this in external files and hold in memory in non-UPROPERTY fields.
//   However, when we mark this object dirty we should tell Unreal so that it knows to show it as unsaved and trigger saving.
//
void UInteractMLStorage::MarkUnsavedData()
{
	//our own flag
	bNeedsSave = true;
}

//~ Begin UObject interface
//good opportunity to load our model/example data too
//
void UInteractMLStorage::PostLoad()
{
	Super::PostLoad();

	//sync up to find the base file path
	SyncAssetWithFile();

	//load our custom data
	Load();

	//this event could affect the derived storage path
	UpdateDerivedState();
}

#if WITH_EDITOR
// we can be notified after an undo (or redo actually)
//
void UInteractMLStorage::PostEditUndo()
{
	//this event could affect the derived storage path
	UpdateDerivedState();

	Super::PostEditUndo();
}

// we can be notified after any editing/manipulation operation
//
void UInteractMLStorage::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	//this event could affect the derived storage path
	UpdateDerivedState();

	Super::PostTransacted(TransactionEvent);
}
#endif

// we can be notified of an asset rename
//
void UInteractMLStorage::PostRename(UObject* OldOuter, const FName OldName)
{
	//this event could affect the derived storage path
	UpdateDerivedState();

	Super::PostRename(OldOuter, OldName);
}

// we can be notified of an import (might be cases where this is called that invalidates our state even though we aren't an importable asset type)
//
void UInteractMLStorage::PostEditImport()
{
	//this event could affect the derived storage path
	UpdateDerivedState();

	Super::PostEditImport();
}

//called once before object is serialised for saving, seems like the best time to save our externally stored data
//
void UInteractMLStorage::PreSave(const class ITargetPlatform* TargetPlatform)
{
	Super::PreSave(TargetPlatform);

	//our save
	if(HasUnsavedData())
	{
		Save();
	}
}

//~ End UObject interface


// extract numerical ID from file
//
FGuid UInteractMLStorage::ExtractGuidFromFile(FString full_file_path)
{
	//skip back past extension if present
	int id_start = full_file_path.Len();
	if (full_file_path.EndsWith(cFileFormatExtension))
	{
		id_start -= cFileFormatExtension.Len();
	}
	
	//skip back past model/example prefix if present
	int prefix_len = UInteractMLTrainingSet::cExtensionPrefix.Len();
	int prefix_start = id_start-prefix_len;
	if (prefix_start >= 0 && FPlatformString::Strnicmp(&full_file_path[prefix_start], *UInteractMLTrainingSet::cExtensionPrefix, prefix_len )==0)
	{
		id_start -= prefix_len;
	}
	FString model_file_extension = GetExtensionPrefix();
	prefix_len = model_file_extension.Len();
	prefix_start = id_start-prefix_len;
	if (prefix_start >= 0 && FPlatformString::Strnicmp(&full_file_path[prefix_start], *model_file_extension, prefix_len )==0)
	{
		id_start -= prefix_len;
	}
	
	//skip back past ID, if present
	if (id_start > cFileIDLength && full_file_path[id_start-cFileIDLength-1] == TCHAR('.'))	//space for it and dot before?
	{
		//...run of hex chars?
		bool is_id = true;
		for (int i=0 ; i<cFileIDLength ; i++)
		{
			id_start--;
			if (!FChar::IsHexDigit(full_file_path[id_start]))
			{
				is_id = false;
				break;
			}
		}
		
		//handle id
		if (is_id)
		{
			FString id_text = full_file_path.Mid(id_start, cFileIDLength);
			FGuid guid;
			if (FGuid::ParseExact(id_text, EGuidFormats::Digits, guid))
			{
				return guid;
			}
		}
	}

	//none/empty
	return FGuid();
}

// ensure just the base path/name are present
//
FString UInteractMLStorage::SanitisePathAndName(FString path_and_name, FString optional_model_type_extension)
{
	//normalise separators
	path_and_name = path_and_name.Replace( TEXT( "\\" ), TEXT( "/" ) );

	//strip root path
	FString root = FInteractMLModule::Get().GetDataRoot();
	if(path_and_name.StartsWith( root ))
	{
		path_and_name = path_and_name.RightChop( root.Len() );
		//leading slash?
		if(path_and_name.StartsWith( TEXT("/") ) )
		{
			path_and_name = path_and_name.RightChop( 1 );
		}
	}

	//remove for json, i.e. ".json"
	if (path_and_name.EndsWith(cFileFormatExtension))
	{
		path_and_name = path_and_name.LeftChop( cFileFormatExtension.Len() );
	}
	
	//remove type, e.g. ".model"
	if (path_and_name.EndsWith( UInteractMLTrainingSet::cExtensionPrefix ))
	{
		path_and_name = path_and_name.LeftChop( UInteractMLTrainingSet::cExtensionPrefix.Len() );
	}
	if (!optional_model_type_extension.IsEmpty() && path_and_name.EndsWith( optional_model_type_extension ))
	{
		path_and_name = path_and_name.LeftChop( optional_model_type_extension.Len() );
	}
	
	//remove ID, e.g. ".5F05C2390B2238A3"
	int len = path_and_name.Len();
	if(path_and_name.Len()>cFileIDLength 
		&& path_and_name[len-cFileIDLength-1]=='.')	//dot followed by...
	{
		//...run of hex chars?
		bool is_id = true;
		for (int i=1 ; i<=cFileIDLength ; i++)
		{
			if (!FChar::IsHexDigit(path_and_name[len-i]))
			{
				is_id = false;
				break;
			}
		}
		
		if (is_id)
		{
			path_and_name = path_and_name.LeftChop( cFileIDLength+1/*dot*/ );
		}
	}

	return path_and_name;
}

//rebuild any derived state, e.g. filename, etc
//
void UInteractMLStorage::UpdateDerivedState()
{
	//only needed for real assets
	//This is because BaseFilePath is set explicitly for temp ones and we don't need a display name
	if (bIsTemporary)
	{
		return;
	}

	//ensure has a guid
	if (!FileID.IsValid())
	{
		FileID = FGuid::NewGuid();
	}

	//rebuild base path (update with asset name)
	FString new_base_path = GetBaseFilePathDirectoryPart();

	//add name on
	FString asset_name;
	GetName(asset_name);
	if (new_base_path.Len() > 0)
	{
		//add to path
		new_base_path = FPaths::Combine(new_base_path, asset_name);
	}
	else
	{
		//just name, in root of data dir
		new_base_path = asset_name;
	}
	
	//changed?
	if(new_base_path!=BaseFilePath)
	{
		UE_LOG(LogInteractML, Log, TEXT("Storage data path changed to '%s' (from '%s')"), *new_base_path, *BaseFilePath);
		BaseFilePath = new_base_path;
	}

	//rebuild display path
	DisplayDataFilePath = BaseFilePath;
	DisplayDataFilePath += ".";
	DisplayDataFilePath += FileID.ToString(EGuidFormats::Digits);
	DisplayDataFilePath += GetExtensionPrefix();	//type prefix
	DisplayDataFilePath += cFileFormatExtension;	//file format extension

	//sync file existance if needed
	SyncFileWithAsset();
}

// ensure file name/path matches the asset, more/rename if not
// returns: true if any change was made to file
bool UInteractMLStorage::SyncFileWithAsset()
{
	//gather
	FString expected_path = GetFilePath();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	//check
	if (PlatformFile.FileExists(*expected_path))
	{
		//file is where expected
		return false;
	}

	//need to relocate the file and move it
	FString file_id = FileID.ToString( EGuidFormats::Digits );
	FString file_extension = file_id + GetExtensionPrefix() + cFileFormatExtension;
	FString root_directory = FInteractMLModule::Get().GetDataRoot();
	TArray<FString> found_paths;
	PlatformFile.FindFilesRecursively(found_paths, *root_directory, *file_extension);
	if (found_paths.Num()!=1)
	{
		if (found_paths.Num() > 1)
		{
			UE_LOG(LogInteractML, Warning, TEXT("Found more than one data file with an ID of '%s', please fix and restart."), *file_id );
		}

		//no file is fine, just not been saved yet
		return false;
	}

	//need to rename (base/asset name is truth on save)
	FString found_path = found_paths[0];
	if (!PlatformFile.MoveFile( *expected_path, *found_path))
	{
		UE_LOG(LogInteractML, Warning, TEXT("Failed to move data file '%s' to '%s', please fix and restart"), *found_path, *expected_path );
		return false;
	}

	//moved
	return true;
}

// ensure our loaded state matches where the file is located
// specifically, find out what the file base path is
// returns: true if asset requires a rename to match the data file
//
bool UInteractMLStorage::SyncAssetWithFile()
{
	//gather
	FString asset_name;
	GetName(asset_name);
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	//locate the file for our data
	FString file_id = FileID.ToString( EGuidFormats::Digits );
	FString file_extension = TEXT(".") + file_id + GetExtensionPrefix() + cFileFormatExtension;
	FString root_directory = FInteractMLModule::Get().GetDataRoot();
	TArray<FString> found_paths;
	PlatformFile.FindFilesRecursively(found_paths, *root_directory, *file_extension);
	if (found_paths.Num()!=1)
	{
		if (found_paths.Num() > 1)
		{
			UE_LOG(LogInteractML, Warning, TEXT("Found more than one data file with an ID of '%s', please fix and restart."), *file_id );
		}
		
		//no file has been saved yet, assume root, adopt asset name
		BaseFilePath = asset_name;
		return false;
	}

	//adopt sub-dir as base for filepath
	FString found_path = found_paths[0];
	found_path = found_path.RightChop(root_directory.Len());	//remove root
	found_path = found_path.LeftChop(file_extension.Len());		//remove extension
	if(found_path.StartsWith( "/" ) || found_path.StartsWith( "\\" ))
	{
		found_path = found_path.RightChop( 1 );
	}
	BaseFilePath = found_path;

	//check for name change, rename asset to follow file (assume file is truth on loading)
	FString file_name_part = GetBaseFilePathNamePart();
	if (asset_name.Compare(file_name_part, ESearchCase::IgnoreCase) != 0)
	{
		//too complicated to manage an asset rename automatically (possible implications/side-effects)
		//just warn instead
		UE_LOG(LogInteractML, Warning, TEXT("Asset name '%s' doesn't match data file name on disk '%s'"), *asset_name, *GetFilePath() );
		return true;
	}

	return false;
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

