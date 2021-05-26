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
	FString m_DataRootPath;


public:
	//systems
	static FInteractMLModule* GetModule() { return s_pModule; }

	//access
	FString GetDataRoot() const { return m_DataRootPath; }


	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	//setup
	void InitPaths();

};
