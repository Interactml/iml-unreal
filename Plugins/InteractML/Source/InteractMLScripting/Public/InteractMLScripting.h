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


// InteractML scripting support module
//
class INTERACTMLSCRIPTING_API FInteractMLScriptingModule 
	: public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

};
