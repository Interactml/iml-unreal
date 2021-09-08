//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLLabelCache.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLLabel.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// LOCAL CLASSES & TYPES

//////////////////////////// LABEL CAPTURE /////////////////////////////

// label data comparison, are the two equal?
//
bool FInteractMLLabelCapture::Equal(TArray<float>& raw_data) const
{
	return LabelData == raw_data;
}


///////////////////////////// LABEL CACHE ////////////////////////////////

// clear out cache of all label captures
// NOTE: reverts it to simple type (no UInteractMLLabel type object)
//
void FInteractMLLabelCache::Reset()
{
	LabelType = nullptr;
	Labels.Empty();
	StringsMap.Empty();
}

// copy state from another cache
// NOTE: Used when propagating label info from training set to model (during training), as model will need the labels to provide corresponding output
//
void FInteractMLLabelCache::Assign(const FInteractMLLabelCache& source)
{
	//full copy (assume we need all of it for all model types for now)
	//TODO: Might not need to copy (and persist) all label info in the model for some model types
	LabelType = source.LabelType;
	Labels = source.Labels;
	StringsMap = source.StringsMap;
}

// resolve a specific label value (set) and get it's associated numeric label
//
float FInteractMLLabelCache::Find(const UInteractMLLabel* label_type, const void *label_data, bool create_if_missing)
{
	check(label_type);
	check(label_data);
	
	//what does the label data actually look like?
	TArray<float> raw_data;
	label_type->CaptureData( label_data, raw_data, *this );
	
	//find existing label
	for (int i = 0; i < Labels.Num(); i++)
	{
		if (Labels[i].Equal(raw_data))
		{
			return (float)i;
		}
	}
	
	//not found is an error as we don't want one created for us
	return -1;
}

// resolve or add/cache a specific label value (set) and get it's associated numeric label
//
float FInteractMLLabelCache::FindOrAdd(const UInteractMLLabel* label_type, const void *label_data)
{
	check(label_type);
	check(label_data);

	//what does the label data actually look like?
	TArray<float> raw_data;
	label_type->CaptureData( label_data, raw_data, *this );

	//find existing label
	for (int i = 0; i < Labels.Num(); i++)
	{
		if (Labels[i].Equal(raw_data))
		{
			return (float)i;
		}
	}

	//type?
	if (LabelType)
	{
		//ensure only one type used in the training set
		if (!label_type->Equal( LabelType ))
		{
			UE_LOG(LogInteractML, Error, TEXT("Attempt to use mixed labels in training data, can't mix '%s' when already using '%s'"), *label_type->GetName(), *LabelType->GetName() );
			return -1;
		}
	}
	else
	{
		//we are this type of label now
		LabelType = label_type;
	}

	//cache new entry
	FInteractMLLabelCapture label( raw_data );  

	//store
	int index = Labels.Num();
	Labels.Add(label);

	return index;
}

// look up/cache strings for each string property slot, mapping to an identifying value (for mapping in recognition phase later)
//
float FInteractMLLabelCache::FindString(const FText& string_value, int property_slot)
{
	for (int i = 0; i < StringsMap.Num(); i++)
	{
		FInteractMLStringMapping& string_entry = StringsMap[i];
		if (string_entry.PropertySlot == property_slot)
		{
			//found map, check strings
			for (int s = 0; s < string_entry.Strings.Num(); s++)
			{
				if (string_entry.Strings[s].EqualTo( string_value ))
				{
					//found existing, this is the index to use
					return (float)s;
				}
			}

			//not found, need to add
			int index = string_entry.Strings.Num();
			string_entry.Strings.Add(string_value);
			return (float)index;
		}
	}

	//no map entry for this slot yet
	FInteractMLStringMapping ms;
	ms.PropertySlot = property_slot;
	ms.Strings.Add(string_value);
	StringsMap.Add( ms );
	//implicitly first entry for this slot, hence index 0
	return 0;
}

// how many float values does this label map to/contain?
//
int FInteractMLLabelCache::GetNumValues() const
{ 
	if (LabelType)
	{
		return LabelType->GetCaptureCount();
	}

	//no type means simple label (one float)
	return 1;
}

// get label values for specific label index
//
bool FInteractMLLabelCache::GetLabel(int label_index, TArray<float>& out_values) const
{
	//find existing label
	if (label_index >= 0 && label_index < Labels.Num())
	{
		//copy out
		out_values = Labels[label_index].LabelData;
		return true;
	}

	//not found
	return false;
}

// get string value cached for specific property slot
//
FText FInteractMLLabelCache::GetString(int property_slot, float label_value) const
{
	for (int s = 0; s < StringsMap.Num(); s++)
	{
		if (StringsMap[s].PropertySlot == property_slot)
		{
			//yes we have a slot
			const TArray<FText>& strings = StringsMap[s].Strings;
			if (strings.Num() > 0)
			{
				//resolve string index
				int index = FMath::RoundToInt(label_value);			//map to nearest
				index = FMath::Clamp(index, 0, strings.Num()-1);	//ensure in range (outside range map to first/last FWIW)
				return strings[index];
			}
		}
	}

	//no string found
	return FText();
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

