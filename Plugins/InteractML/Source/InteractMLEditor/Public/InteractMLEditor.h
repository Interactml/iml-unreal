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
	, public IHasToolBarExtensibility
{
	// asset appearance
	TSharedPtr<FSlateStyleSet> AssetStyleSet;

	// systems
	static class FInteractMLEditorModule* EditorModule;

	/** Extensibility managers */
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
	
public:

	// systems access
	static class FInteractMLEditorModule* GetModule() { return EditorModule; }
	static TSharedPtr<FSlateStyleSet> GetStyle() { return EditorModule->AssetStyleSet; }
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** IHasToolBarExtensibility interface */
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }	
	
private:

	// editor module setup/shutdown
	void InitHooks();
	void InitAssets();
	void InitExtensibility();
		
	void ShutdownAssets();
	void ShutdownHooks();
	void ShutdownExtensibility();

	// global editor event handlers	
	void OnEndPIE(const bool bIsSimulating);
	void OnWorldSaved(uint32 SaveFlags, UWorld* World, bool bSuccess);
	
};
