//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"

//module
#include "InteractMLHelpers.h"
#include "InteractMLLabel.generated.h"

//general declarations

// InteractML Label type (structure definition)
// List of name/type pairs defining the set of values to be associated together as a label
// Used for specifying expected output when recording/training and produced as actual output on a running model
// NOTE: Just a simple specialisation of Blueprint Struct asset type, we want all the functionality/features but don't really need to add much
//
UCLASS()
class INTERACTML_API UInteractMLLabel : public UUserDefinedStruct
{
	GENERATED_BODY()
public:


	// capture the fields of an instance of this struct type into an array of floats
	//
	void CaptureData( const void* in_struct_instance, TArray<float>& out_float_data, struct FInteractMLLabelCache& label_cache ) const;


	// does this label type have the same structure as us?
	//
	bool Equal(const UInteractMLLabel* other) const;



private:
	
	//internals
	int CaptureData(FField* prop, int iprop, const uint8* in_struct_instance, TArray<float>& out_float_data, struct FInteractMLLabelCache& label_cache) const;
	
};
