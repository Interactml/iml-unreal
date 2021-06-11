//----
// InteractML - University Of Arts London
//----

#include "InteractMLTrainingSetSnapshotNode.h"

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



//////////////////// SNAPSHOT TRAINING SET SPECIALISATION //////////////////////


FText UInteractMLTrainingSetSnapshotNode::GetTrainingSetName() const
{
	return LOCTEXT("TrainingSetlNameSnapshot", "Single");
}
FText UInteractMLTrainingSetSnapshotNode::GetTrainingSetDescription() const
{
	return LOCTEXT("TrainingSetlNameSnapshot", "Record single sample snaphot");
}
FText UInteractMLTrainingSetSnapshotNode::GetTrainingSetTooltip() const
{
	return LOCTEXT("TrainingSetTooltipSnapshot", "Record a single parameter snapshot at a time for the current label");
}
FText UInteractMLTrainingSetSnapshotNode::GetRecordPinTooltip() const
{
	return LOCTEXT("TrainingSetRecordPinTooltipSnapshot", "Set to true to record a single example snapshot for the current label.");
}
FName UInteractMLTrainingSetSnapshotNode::GetRecordFunctionName() const
{
	static const FName record(GET_FUNCTION_NAME_CHECKED(UInteractMLBlueprintLibrary, RecordExample));
	return record;
}




#undef LOCTEXT_NAMESPACE
