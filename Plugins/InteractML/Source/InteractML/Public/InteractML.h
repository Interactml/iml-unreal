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

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

};
