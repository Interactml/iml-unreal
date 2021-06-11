//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal

//module
#include "InteractMLTrainingSetNode.h"
#include "InteractMLTrainingSetSeriesNode.generated.h"

//general declarations

// Training set specialisation for parameter series recording
//
UCLASS()
class INTERACTMLSCRIPTING_API UInteractMLTrainingSetSeriesNode: public UInteractMLTrainingSetNode
{
	GENERATED_BODY()

public:

private:
	
protected:

	//~ Begin training-set specific overrides
	virtual FText GetTrainingSetName() const override;
	virtual FText GetTrainingSetDescription() const override;
	virtual FText GetTrainingSetTooltip() const override;
	virtual FName GetRecordFunctionName() const override;
	//~ End training-set specific overrides
		
private:
	
};