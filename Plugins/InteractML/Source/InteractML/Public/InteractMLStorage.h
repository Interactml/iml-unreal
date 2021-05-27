//----
// InteractML - University Of Arts London
//----

#pragma once


//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLStorage.generated.h"

//general declarations


// InteractML data storage base class
// Handles interaction with the underlying JSON data file used to persist the ML data (training or trained model))
//
UCLASS()
class INTERACTML_API UInteractMLStorage
	: public UObject
{
	GENERATED_BODY()
		
	//---- persistent state ----

	//hard link to underlying data file, if one is present in the filename
	UPROPERTY()
	FGuid FileID;

	
	//---- transient/cached state ----

	//actual root of file path, e.g. Examples/HandGestures, the rest comes from ID (if it has one) and actual type
	FString BaseFilePath;
	
	
public:
	//---- constants ----

	// json format file extension
	static FString cFileFormatExtension;
	
	// number of hex digits expected in filename embedded IDs (e.g. MyModel.51B05C30F9A0E984.model.json)
	static const int cFileIDLength = 32; //big enough for a binary GUID

	//---- access ----

	// each type provides qualifying extension prefix
	virtual FString GetExtensionPrefix() const { check(false); return FString(); }

	// request the full file path to the backing storage for this objects ML data
	FString GetFilePath() const;      //e.g. D:/MLProject/Content/Examples/HandGestures.19D38579C13759B1.training.json, although pure file-based ones may not have an ID
	FString GetDirectoryPath() const; //e.g. D:/MLProject/Content/Examples/

	//---- persistence ----

	// load existing ML state from disk
	virtual bool Load(); //override Load to completely control loading of Json file data
	virtual bool LoadJson(/*todo*/) { check(false); return false; } //or just override LoadJson to just parse Json

	// persist current ML state to disk
	virtual bool Save(); //override Save to completely control saving of Json file data
	virtual bool SaveJson(/*todo*/) { check(false); return false; } //or just override SaveJson to just have to generate Json

	
	//---- utility ----

	// extract/ensure just path and base file name from a path
	static FString SanitisePathAndName( FString path_and_name );
		
	// extract numerical ID from file
	static FGuid ExtractGuidFromFile(FString full_file_path);

private:
	//internal access
	friend class FInteractMLModule;
	
	//various setup happens when you set the base_file_path
	void FInteracMLModule_SetBaseFilePath( FString base_file_path );
	
};
