//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"

//module
#include "InteractMLHelpers.h"
#include "InteractMLLabel.generated.h"

//general declarations

// InteractML Label type (data driven structure definition)
// List of name/type pairs defining the set of values to be associated together as a label
// Used for specifying expected output when recording/training and produced as actual output on a running model
// NOTE: Just a simple specialisation of Blueprint Struct asset type, we want all the functionality/features but don't really need to add much
//
UCLASS(hidecategories=Object, MinimalAPI, BlueprintType)
class UInteractMLLabel : public UUserDefinedStruct
{
	GENERATED_BODY()
	
	/////////// DERIVED/NON-PERSISTENT ///////////

	//number of floats this label will capture
	mutable int CaptureCount;

public:

	// number of floats this label will capture
	int GetCaptureCount() const { ValidateDerived(); return CaptureCount; }

	// capture the fields of an instance of this struct type into an array of floats
	void CaptureData( const void* in_struct_instance, TArray<float>& out_float_data, struct FInteractMLLabelCache& label_cache ) const;

	// rebuild a structure from set of field values
	void RecreateData(const TArray<float>& label_values, void* out_struct_instance, const struct FInteractMLLabelCache& label_cache ) const;
	
	// does this label type have the same structure as us?
	bool Equal(const UInteractMLLabel* other) const;

#if WITH_EDITOR
	// UObject interface.
	virtual bool Modify(bool bAlwaysMarkDirty)
	{
		//edited
		InvalidateDeferred();
		return Super::Modify(bAlwaysMarkDirty);
	}
	// End of UObject interface.
#endif

private:
	
	///////////// INTERNALS //////////////

	//capture a structure field (for recursion into embedded structures like FVector or FColor)
	void CaptureData(FField* prop, int& iprop, const uint8* in_struct_instance, TArray<float>& out_float_data, struct FInteractMLLabelCache& label_cache) const;

	//rebuild structure from field values (for recursion into embedded structures like FVector or FColor)
	void RecreateData(FField* prop, int& iprop, const TArray<float>& label_values, int& value_index, uint8* out_struct_instance, const struct FInteractMLLabelCache& label_cache) const;

	//count float values a structures fields will capture
	int CountValues(FField* prop) const;

	//invalidate cached deferred state, will rebuild when next needed
	void InvalidateDeferred()
	{
		CaptureCount = 0;
	}

	//rebuild any derived state
	void ValidateDerived() const;
};
