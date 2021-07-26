//----
// InteractML - University Of Arts London
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

// resolve/cache a specific label value (set) and get it's associated numeric label
//
float FInteractMLLabelCache::Find(const UInteractMLLabel* label_type, const void *label_data)
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
			UE_LOG(LogInteractML, Error, TEXT("Attempt to use mixed labels in training data, can't mix '%s' when already using '%s'"), *label_type->GetDisplayNameText().ToString(), *LabelType->GetDisplayNameText().ToString() );
			return 0;
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
float FInteractMLLabelCache::FindString(const FString& string_value, int property_slot)
{
	for (int i = 0; i < StringsMap.Num(); i++)
	{
		FInteractMLStringMapping& string_entry = StringsMap[i];
		if (string_entry.PropertySlot == property_slot)
		{
			//found map, check strings
			for (int s = 0; s < string_entry.Strings.Num(); s++)
			{
				if (string_entry.Strings[s] == string_value)
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


// EPILOGUE
#undef LOCTEXT_NAMESPACE

