//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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
