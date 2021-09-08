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
#include "InteractMLLabelCache.generated.h"

//general declarations


// captured single set of label values
USTRUCT()
struct INTERACTML_API FInteractMLLabelCapture
{
	GENERATED_BODY()

	//label data capture (flattened to float list
	UPROPERTY()
	TArray<float> LabelData;


public:

	FInteractMLLabelCapture() {}
	FInteractMLLabelCapture(TArray<float>& raw_data)
	: LabelData(raw_data) {}
	
	//is this raw data equivalent to this captured data?
	bool Equal(TArray<float>& raw_data) const;
};


// string cache map
USTRUCT()
struct INTERACTML_API FInteractMLStringMapping
{
	GENERATED_BODY()

	// the slot in the structure these strings belong to
	UPROPERTY()
	int PropertySlot;

	// list of strings, identified by index
	// (generally assumed const in nature so stored as FText)
	UPROPERTY()
	TArray<FText> Strings;
};


// InteractML Label Cache
// For remembering the association between composite label type and each set of label data recorded in a training set
//
USTRUCT()
struct INTERACTML_API FInteractMLLabelCache
{
	GENERATED_BODY()

public:
	//the type of composite label we are caching for (simple numeric/float label assumed if this not set)
	UPROPERTY()
	const class UInteractMLLabel* LabelType;

	//labels accumulated so far, each recording could have a unique label associated
	//NOTE: index in array is the numerical label used in training discrete models, continuous models use the whole capture
	UPROPERTY()
	TArray<FInteractMLLabelCapture> Labels;

	//strings are stored in the cache and mapped to an index value that can be fed through the ML system
	//and then resolved back to a string at the other end.  Fine for discrete models, but a bit questionably for
	//continuous models as you can't interpolate strings, but selecting the nearest by index will have to suffice.
	UPROPERTY()
	TArray<FInteractMLStringMapping> StringsMap;

private:

public:

	///////////////// ACCESS /////////////////

	//how many float values does this label map to/contain?
	int GetNumValues() const;

	/////////////// OPERATION ///////////////

	//clear out cache of all label captures
	void Reset();

	//take on the cached state from another cache
	void Assign( const FInteractMLLabelCache& source );

	///////////////// QUERY /////////////////

	//resolve a specific label value (set) and get it's associated numeric label
	float Find(const class UInteractMLLabel* label_type, const void *label_data, bool create_if_missing );

	//resolve or add/cache a specific label value (set) and get it's associated numeric label
	float FindOrAdd(const class UInteractMLLabel* label_type, const void *label_data );

	// look up/cache strings for each string property slot, mapping to an identifying value (for mapping in recognition phase later)
	float FindString( const FText& string_value, int property_slot );
	
	// get label values for specific label index
	bool GetLabel(int label_index, TArray<float>& out_values) const;

	// lookup a string previously cached
	FText GetString(int property_slot, float label_value) const;

private:
	
};
