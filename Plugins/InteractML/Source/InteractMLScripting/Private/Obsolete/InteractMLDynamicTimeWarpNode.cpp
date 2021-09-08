//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLDynamicTimeWarpNode.h"

//unreal
//#include "BlueprintActionDatabaseRegistrar.h" //FBlueprintActionDatabaseRegistrar
//#include "BlueprintNodeSpawner.h" //UBlueprintNodeSpawner
//#include "EdGraphSchema_K2.h" //UEdGraphSchema_K2
//#include "KismetCompiler.h" //FKismetCompilerContext
//#include "K2Node_CallFunction.h" //UK2Node_Function
//#include "Engine/SimpleConstructionScript.h" //USimpleConstructionScript
//#include "BlueprintEditorUtils.h" //MarkBlueprintAsStructurallyModified
//#include "ToolMenu.h" //UToolMenu
//#include "ScopedTransaction.h" //FScopedTransaction

//module
//#include "InteractMLTrainingSet.h"
#include "InteractMLBlueprintLibrary.h"
//#include "InteractMLConstants.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES



/////////////////////////////////// HELPERS /////////////////////////////////////



/////////////////// DYNAMIC TIMEWARP MODEL NODE SPECIALISATION //////////////////


FText UInteractMLDynamicTimeWarpNode::GetModelName() const
{
	return LOCTEXT("ModelNameDynamicTimeWarp", "Dynamic Time Warp");
}
FText UInteractMLDynamicTimeWarpNode::GetModelTooltip() const
{
	return LOCTEXT("ModelTooltipDynamicTimeWarp", "Load or train a dynamic time-warp machine learning model");
}
FName UInteractMLDynamicTimeWarpNode::GetModelAccessFunctionName() const
{
	static const FName get_model(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, DEPRECATED_GetModel_DynamicTimeWarp));
	return get_model;
}




#undef LOCTEXT_NAMESPACE
