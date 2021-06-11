//----
// InteractML - University Of Arts London
//----

#include "InteractMLRegressionNode.h"

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



//////////////////// CLASSIFICATION MODEL NODE SPECIALISATION ///////////////////


FText UInteractMLRegressionNode::GetModelName() const
{
	return LOCTEXT("ModelNameRegression", "Regression");
}
FText UInteractMLRegressionNode::GetModelTooltip() const
{
	return LOCTEXT("ModelTooltipRegression", "Load or train a regression machine learning model");
}
FName UInteractMLRegressionNode::GetModelAccessFunctionName() const
{
	static const FName get_model(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, GetModel_Regression));
	return get_model;
}





#undef LOCTEXT_NAMESPACE
