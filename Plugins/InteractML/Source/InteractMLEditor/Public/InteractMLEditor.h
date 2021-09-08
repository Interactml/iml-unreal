//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Styling/SlateStyle.h"

//module
#include "InteractML.h"

// general declarations


// InteractML editor only module
//
class INTERACTMLEDITOR_API FInteractMLEditorModule 
	: public IModuleInterface
{
	// asset appearance
	TSharedPtr<FSlateStyleSet> AssetStyleSet;
	
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	// editor module setup/shutdown
	void InitHooks();
	void InitAssets();
	
	void ShutdownAssets();
	void ShutdownHooks();

	// global editor event handlers	
	void OnEndPIE(const bool bIsSimulating);
	void OnWorldSaved(uint32 SaveFlags, UWorld* World, bool bSuccess);
	
};
