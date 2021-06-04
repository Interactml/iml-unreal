//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
//#include "Containers/Tickers.h"

//module
#include "InteractML.h"

// general declarations


// InteractML editor only module
//
class INTERACTMLEDITOR_API FInteractMLEditorModule 
	: public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	// editor module setup/shutdown
	void InitHooks();
	void ShutdownHooks();

	// global editor event handlers	
	void OnEndPIE(const bool bIsSimulating);
	void OnWorldSaved(uint32 SaveFlags, UWorld* World, bool bSuccess);
	
};
