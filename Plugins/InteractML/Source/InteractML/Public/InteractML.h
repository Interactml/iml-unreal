//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Containers/Ticker.h"
#include "Misc/ConfigCacheIni.h"

//module
#include "Modules/ModuleInterface.h"
#include "InteractMLTask.h"


// CONFIGURATION

//enable/disable multi-threaded operations (assuming nodes are set to use it)
#define INTERACTML_ALLOW_MULTITHREADING		1

// we can only use C++ exceptions in the editor builds, not at runtime, Unreal doesn't support it (perf reasons)
#if WITH_EDITOR
#define INTERACTML_TRAP_CPP_EXCEPTIONS 		1
#else
#define INTERACTML_TRAP_CPP_EXCEPTIONS		0
#endif


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
	TMap<FString, TWeakObjectPtr<class UInteractMLStorage>> ObjectLookup;

	//tick
	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;

	//async support
	TArray<FInteractMLTask::Ptr>	PendingTasks;
	TArray<FInteractMLTask::Ptr>	CompletedTasks;
	FCriticalSection				CompletedTaskInterlock;

public:
	//systems
	static FInteractMLModule& Get() { return *s_pModule; }

	//access
	FString GetDataRoot() const { return DataRootPath; }
	bool HasUnsavedData() const; //anything in unsaved temp objects? (handled differently to assets)
	bool GetUnsavedAssets(TArray<class UInteractMLStorage*>& changed_assets); //anything in unsaved assets?

	//state
	bool Save(); //save any unsaved training or model data
	
	//ml objects : obtain path based ones here to track and synchronise globally
	class UInteractMLTrainingSet* GetTrainingSet( FString path_and_name );			//note, just [Path/]Name
	class UInteractMLModel* GetModel( UClass* model_type, FString path_and_name );	//note, just [Path/]Name

	//ml objects : inform of any obtained from direct asset references here as we need to synchronise with path based ones
	void SetTrainingSet( class UInteractMLTrainingSet* training_set );
	void SetModel( class UInteractMLModel* model );

	//async execution
	void RunTask( FInteractMLTask::Ptr task );
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	//setup
	void InitTick();
	void InitPaths();
	
	//update
	bool Tick(float DeltaTime);
	void TickTasks(float dt);
		
	//shutdown
	void ShutdownTick();
	void ShutdownCache();
	void ShutdownTasks();

};
