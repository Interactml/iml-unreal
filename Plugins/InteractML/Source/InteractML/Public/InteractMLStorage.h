//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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
class INTERACTML_API UInteractMLStorage	: public UObject
{
	GENERATED_BODY()

	//---- persistent state ----

	//hard link to underlying data file, if one is present in the filename
	UPROPERTY()
	FGuid FileID;

	//---- transient/cached state ----

	//was created as temporary interactml object used by nodes that specific data file path explicitly
	//these are not project assets, and are not persisted, they only act as wrappers for the raw json data representation
	UPROPERTY(Transient)
	bool bIsTemporary;

	//file path/id/name/ext/etc this is derived from file search and object state and not editable
	//TODO: Spot changes to file location and update/sync, update file when object renamed
	UPROPERTY(VisibleAnywhere)
	FString DisplayDataFilePath;

	//actual root of file path, e.g. Examples/HandGestures, the rest comes from ID (if it has one) and actual type
	//case 1: set explicitly for temporary storage objects (data path access on nodes)
	//case 2: derived from object name and current file directory location (actual asset file access)
	FString BaseFilePath;

	//has unsaved state?
	mutable bool bNeedsSave;

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

	// request the full directory path the backing storage for this objects ML data file is located in
	FString GetDirectoryPath() const; //e.g. D:/MLProject/Content/Examples/

	// is the base path being used for storage?
	bool CheckBasePath(FString base_file_path) const;

	// any sub-dir part of base file path, e.g. "Examples/Foo/HandGestures" -> "Examples/Foo", and "Hello" -> ""
	FString GetBaseFilePathDirectoryPart() const;

	// the name part of base file path, e.g. "Examples/Foo/HandGestures" -> "HandGestures", and "Hello" -> "Hello"
	FString GetBaseFilePathNamePart() const;

	// fill base file path
	FString GetBaseFilePath() const { return BaseFilePath; }
	
	//---- persistence ----

	// a new storage object, chance to init when not loaded
	virtual void Create() {}
	
	// load existing ML state from disk
	virtual bool Load(); //override Load to completely control loading of Json file data
	virtual bool LoadJson( const FString& json_string ) { check(false); return false; } //or override LoadJson to just parse Json

	// persist current ML state to disk
	virtual bool Save() const; //override Save to completely control saving of Json file data
	virtual bool SaveJson( FString& json_string ) const { check(false); return false; } //or override SaveJson to just have to generate Json

	// is there unsaved state in this object?
	bool HasUnsavedData() const { return bNeedsSave; }
	void MarkUnsavedData();

	//~ Begin UObject interface
	//events that could affect the derived storage path
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditUndo() override;
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;
#endif
	virtual void PostRename(UObject* OldOuter, const FName OldName) override;
	virtual void PostEditImport() override;
	//save hook
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;
	//~ End UObject interface
	
	//---- utility ----

	// extract/ensure just path and base file name from a path
	static FString SanitisePathAndName( FString path_and_name, FString optional_model_type_extension="" );
		
	// extract numerical ID from file
	FGuid ExtractGuidFromFile(FString full_file_path);

private:
	//internal access
	friend class FInteractMLModule;
	friend class UInteractMLTrainingSetFactory;
	friend class UInteractMLClassificationModelFactory;
	friend class UInteractMLRegressionModelFactory;
	friend class UInteractMLDynamicTimewarpModelFactory;
	
	//various setup happens when you set the base_file_path
	void FInteracMLModule_SetBaseFilePath( FString base_file_path );

	//rebuild any derived state, e.g. filename, etc
	void UpdateDerivedState();

	//ensure file name/path matches the asset, more/rename if not
	bool SyncFileWithAsset();

	//ensure our loaded state matches where the file is located
	bool SyncAssetWithFile();
	
};
