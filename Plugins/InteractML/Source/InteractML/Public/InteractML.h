//----
// InteractML - University Of Arts London
//----

#pragma once

//module
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

//unreal
//#include "Containers/Tickers.h"


// general declarations
DECLARE_LOG_CATEGORY_EXTERN(LogInteractML, Log, All);


// InteractML runtime module
//
class INTERACTML_API FInteractMLModule 
	: public IModuleInterface
{
	//systems
	static FInteractMLModule* s_pModule;

	// root location of persisted/imported project ML data
	FString DataRootPath;

	// catalogue of IML objects in use
	TMap<FString, class UInteractMLStorage*> ObjectLookup;

public:
	//systems
	static FInteractMLModule& Get() { return *s_pModule; }

	//access
	FString GetDataRoot() const { return DataRootPath; }
	bool HasUnsavedData() const; //anything unsaved?

	//state
	void Save(); //save any unsaved training or model data
	
	//ml objects : obtain path based ones here to track and synchronise globally
	class UInteractMLTrainingSet* GetTrainingSet( FString path_and_name );			//note, just [Path/]Name
	class UInteractMLModel* GetModel( UClass* model_type, FString path_and_name );	//note, just [Path/]Name

	//ml objects : inform of any obtained from direct asset references here as we need to synchronise with path based ones
	void SetTrainingSet( class UInteractMLTrainingSet* training_set );
	void SetModel( class UInteractMLModel* model );
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	//setup
	void InitPaths();

	//shutdown
	void ShutdownCache();
	
};
