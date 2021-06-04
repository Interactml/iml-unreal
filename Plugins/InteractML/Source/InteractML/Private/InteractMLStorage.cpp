//----
// InteractML - University Of Arts London
//----

#include "InteractMLStorage.h"

//unreal

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
	file_system.FindFilesRecursively( all_data_files, *root_path, *full_extension );
	for (int i = 0; i < all_data_files.Num(); i++)
	{
		//compare base path/name only for match
		FString file_path = all_data_files[i];		
		FString just_path_and_name = SanitisePathAndName(file_path);
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
	}
}

// is the base path being used for storage?
bool UInteractMLStorage::CheckBasePath(FString base_file_path) const
{
	base_file_path = SanitisePathAndName( base_file_path );
	return BaseFilePath.Compare( base_file_path, ESearchCase::IgnoreCase ) == 0;
}

// build full directory to location of the data file
// e.g. D:/MLProject/Content/Examples/
//
FString UInteractMLStorage::GetDirectoryPath() const
{
	FString dir;
	
	//root
	dir = FInteractMLModule::Get().GetDataRoot();
	
	//base path (if there is one)
	check(BaseFilePath.Len() > 0);
	FString dir_part = BaseFilePath;
	int ifs=-1, ibs=-1;
	dir_part.FindLastChar(TCHAR('/'), ifs);
	dir_part.FindLastChar(TCHAR('\\'), ibs);
	int iname = FMath::Max(ifs+1, ibs+1);
	dir_part = dir_part.Left(iname);
	
	dir = FPaths::Combine(dir, dir_part);
	return dir;
}

// build full file path for storage
// e.g. D:/MLProject/Content/Examples/HandGestures.19D38579C13759B1.training.json, although pure file-based ones may not have an ID
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
	FStringView whole_string( json_string );
	if(FFileHelper::SaveStringToFile( whole_string, *path ))
	{
		//nolonger needs save
		bNeedsSave = false;
		return true;
	}
	
	//failed
	return false;
}


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
	prefix_len = UInteractMLModel::cExtensionPrefix.Len();
	prefix_start = id_start-prefix_len;
	if (prefix_start >= 0 && FPlatformString::Strnicmp(&full_file_path[prefix_start], *UInteractMLModel::cExtensionPrefix, prefix_len )==0)
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
FString UInteractMLStorage::SanitisePathAndName(FString path_and_name)
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
	if (path_and_name.EndsWith( UInteractMLModel::cExtensionPrefix ))
	{
		path_and_name = path_and_name.LeftChop( UInteractMLModel::cExtensionPrefix.Len() );
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


// EPILOGUE
#undef LOCTEXT_NAMESPACE

