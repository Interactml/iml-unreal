//----
// InteractML - University Of Arts London
//----

#include "InteractMLLabel.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLParameters.h"
#include "InteractMLLabelCache.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// LOCAL CLASSES & TYPES



// capture the fields of an instance of this struct type into an array of floats
//
void UInteractMLLabel::CaptureData( const void* in_struct_instance, TArray<float>& out_float_data, FInteractMLLabelCache& label_cache ) const
{
	FField* prop = ChildProperties;
	CaptureData(prop, 0, (const uint8*)in_struct_instance, out_float_data, label_cache);
}

// internal capture impl, to allow for recursive struct evaluation
//
int UInteractMLLabel::CaptureData( FField* prop, int iprop, const uint8* in_struct_instance, TArray<float>& out_float_data, FInteractMLLabelCache& label_cache ) const
{
	while (prop)
	{
		//numeric
		if (FNumericProperty* num_prop = CastField<FNumericProperty>(prop))
		{
			const uint8* pdata = in_struct_instance + num_prop->GetOffset_ForUFunction();
			if (num_prop->IsInteger())
			{
				//int/uint/short/ushort/etc
				float value = (float)num_prop->GetSignedIntPropertyValue(pdata);
				out_float_data.Add(value);
			}
			else if (num_prop->IsFloatingPoint())
			{
				//float/double
				float value = (float)num_prop->GetFloatingPointPropertyValue(pdata);
				out_float_data.Add(value);
			}
			else
			{
				//what other sort of numeric prop do we need to support?
				check(false);
			}
		}
		else if (FStructProperty* struct_prop = CastField<FStructProperty>(prop))
		{
			//structures, dig in
			const uint8* pdata = in_struct_instance + struct_prop->GetOffset_ForUFunction();
			iprop = CaptureData( struct_prop->Struct->ChildProperties, iprop, pdata, out_float_data, label_cache);
		}
		else if (FStrProperty* str_prop = CastField<FStrProperty>(prop))
		{
			const uint8* pdata = in_struct_instance + str_prop->GetOffset_ForUFunction();
			const FString& value = str_prop->GetPropertyValue(pdata);
			float string_num = label_cache.FindString( value, iprop );
			out_float_data.Add( string_num );
		}
		else if(FBoolProperty* bool_prop = CastField<FBoolProperty>( prop ))
		{
			const uint8* pdata = in_struct_instance + bool_prop->GetOffset_ForUFunction();
			bool value = bool_prop->GetPropertyValue( pdata );
			out_float_data.Add( value?1.0f:0.0f );
		}
		else
		{
			UE_LOG(LogInteractML, Warning, TEXT("Unable to capture unsupported type '%s' in field '%s' of label structure %s'"), *prop->GetClass()->GetName(), *prop->GetName(), *GetDisplayNameText().ToString() );
		}


		prop = prop->Next;
		iprop++;
	}

	return iprop;
}


// does this label type have the same structure as us?
//
bool UInteractMLLabel::Equal(const UInteractMLLabel* other) const
{
	UField* pmyfield = Children;
	UField* potherfield = other->Children;
	while (pmyfield && potherfield)
	{
		//compare field data types
		if (pmyfield->GetClass() != potherfield->GetClass())
		{
			//found a mismatched field type
			return false;
		}
		pmyfield = pmyfield->Next;
		potherfield = potherfield->Next;
	}
	
	//good if both reached end together
	return pmyfield == nullptr && potherfield == nullptr;
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

