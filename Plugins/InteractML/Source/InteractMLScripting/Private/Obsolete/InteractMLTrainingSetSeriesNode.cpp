//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLTrainingSetSeriesNode.h"

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



////////////////////// SERIES TRAINING SET SPECIALISATION ///////////////////////


FText UInteractMLTrainingSetSeriesNode::GetTrainingSetName() const
{
	return LOCTEXT("TrainingSetlNameSeries", "Series");
}
FText UInteractMLTrainingSetSeriesNode::GetTrainingSetDescription() const
{
	return LOCTEXT("TrainingSetlNameSeries", "Record sample series");
}
FText UInteractMLTrainingSetSeriesNode::GetTrainingSetTooltip() const
{
	return LOCTEXT("TrainingSetTooltipSeries", "Record a series of parameter snapshots in a row for the current label");
}
FText UInteractMLTrainingSetSeriesNode::GetRecordPinTooltip() const
{
	return LOCTEXT("TrainingSetRecordPinTooltipSeries", "Records a series of examples for the current label whilst true.");
}
FName UInteractMLTrainingSetSeriesNode::GetRecordFunctionName() const
{
	static const FName record(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RecordExampleSeries));
	return record;
}




#undef LOCTEXT_NAMESPACE
