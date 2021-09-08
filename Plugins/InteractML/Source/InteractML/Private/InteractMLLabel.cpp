//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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
	int iprop = 0;
	CaptureData(prop, iprop, (const uint8*)in_struct_instance, out_float_data, label_cache);
}

// internal capture impl, to allow for recursive struct evaluation
//
void UInteractMLLabel::CaptureData( FField* prop, int& iprop, const uint8* in_struct_instance, TArray<float>& out_float_data, FInteractMLLabelCache& label_cache ) const
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
				UE_LOG(LogInteractML, Warning, TEXT("Unable to capture unsupported type '%s' in field '%s' of label structure %s'"), *prop->GetClass()->GetName(), *prop->GetName(), *GetName() );
			}
		}
		else if (FStructProperty* struct_prop = CastField<FStructProperty>(prop))
		{
			//structures, dig in
			const uint8* pdata = in_struct_instance + struct_prop->GetOffset_ForUFunction();
			CaptureData( struct_prop->Struct->ChildProperties, iprop, pdata, out_float_data, label_cache);
		}
		else if (FTextProperty* txt_prop = CastField<FTextProperty>(prop))
		{
			const uint8* pdata = in_struct_instance + txt_prop->GetOffset_ForUFunction();
			const FText& value = txt_prop->GetPropertyValue(pdata);
			float string_num = label_cache.FindString( value, iprop );
			out_float_data.Add( string_num );
		}
		else if (FStrProperty* str_prop = CastField<FStrProperty>(prop))
		{
			const uint8* pdata = in_struct_instance + str_prop->GetOffset_ForUFunction();
			const FString& value = str_prop->GetPropertyValue(pdata);
			float string_num = label_cache.FindString( FText::FromString( value ), iprop );
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
			UE_LOG(LogInteractML, Warning, TEXT("Unable to capture unsupported type '%s' in field '%s' of label structure %s'"), *prop->GetClass()->GetName(), *prop->GetName(), *GetName() );
		}


		prop = prop->Next;
		iprop++;
	}
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

//rebuild any derived state
//NOTE: needs to be const as used from accessors and updates on demand
//
void UInteractMLLabel::ValidateDerived() const
{
	//anything needed?
	if (CaptureCount != 0)
	{
		return;
	}

	//rebuild states
	CaptureCount = 	CountValues( ChildProperties );
}

// count float values the fields of an instance of this struct type will capture
//
int UInteractMLLabel::CountValues( FField* prop ) const
{
	int count = 0;
	while (prop)
	{
		//numeric
		if (FNumericProperty* num_prop = CastField<FNumericProperty>(prop))
		{
			count++;
		}
		else if (FStructProperty* struct_prop = CastField<FStructProperty>(prop))
		{
			//structures, dig in
			count += CountValues( struct_prop->Struct->ChildProperties );
		}
		else if (FTextProperty* txt_prop = CastField<FTextProperty>(prop))
		{
			count++;
		}
		else if (FStrProperty* str_prop = CastField<FStrProperty>(prop))
		{
			count++;
		}
		else if(FBoolProperty* bool_prop = CastField<FBoolProperty>( prop ))
		{
			count++;
		}
		else
		{
			UE_LOG(LogInteractML, Warning, TEXT("Unable to capture unsupported type '%s' in field '%s' of label structure %s'"), *prop->GetClass()->GetName(), *prop->GetName(), *GetName() );
		}
				
		prop = prop->Next;
	}
	
	return count;
}

// capture the fields of an instance of this struct type into an array of floats
//
void UInteractMLLabel::RecreateData( const TArray<float>& label_values, void* out_struct_instance, const FInteractMLLabelCache& label_cache ) const
{
	//ensure enough props available
	if (GetCaptureCount()!=label_values.Num())
	{
		UE_LOG(LogInteractML, Error, TEXT("Label definition changed since model trained, model produced %i values whilst %s label expects %i values"), label_values.Num(), *GetName(), GetCaptureCount() );
		return;
	}

	//rebuild
	FField* prop = ChildProperties;
	int iprop = 0;
	int ivalue = 0;
	RecreateData(prop, iprop, label_values, ivalue, (uint8*)out_struct_instance, label_cache);
}

// internal recreation impl, to allow for recursive struct evaluation
//
void UInteractMLLabel::RecreateData(FField* prop, int& iprop, const TArray<float>& label_values, int& ivalue, uint8* out_struct_instance, const FInteractMLLabelCache& label_cache) const
{
	while (prop)
	{
		//numeric
		if (FNumericProperty* num_prop = CastField<FNumericProperty>(prop))
		{
			uint8* pdata = out_struct_instance + num_prop->GetOffset_ForUFunction();
			if (num_prop->IsInteger())
			{
				//int/uint/short/ushort/etc
				int32 value = FMath::RoundToInt(label_values[ivalue]);
				num_prop->SetIntPropertyValue(pdata, (int64)value);
				ivalue++;
			}
			else if (num_prop->IsFloatingPoint())
			{
				//float/double
				float value = label_values[ivalue];
				num_prop->SetFloatingPointPropertyValue(pdata, value);
				ivalue++;
			}
			else
			{
				//what other sort of numeric prop do we need to support?
				UE_LOG(LogInteractML, Warning, TEXT("Unable to recreate unsupported type '%s' in field '%s' of label structure %s'"), *prop->GetClass()->GetName(), *prop->GetName(), *GetName());
			}
		}
		else if (FStructProperty* struct_prop = CastField<FStructProperty>(prop))
		{
			//structures, dig in
			uint8* pdata = out_struct_instance + struct_prop->GetOffset_ForUFunction();
			RecreateData(struct_prop->Struct->ChildProperties, iprop, label_values, ivalue, pdata, label_cache);
		}
		else if (FTextProperty* txt_prop = CastField<FTextProperty>(prop))
		{
			uint8* pdata = out_struct_instance + txt_prop->GetOffset_ForUFunction();
			//find nearest string
			int32 value = FMath::RoundToInt(label_values[ivalue]);
			const FText label_string = label_cache.GetString(iprop, value);
			//apply
			txt_prop->SetPropertyValue(pdata, label_string);
			ivalue++;
		}
		else if (FStrProperty* str_prop = CastField<FStrProperty>(prop))
		{
			uint8* pdata = out_struct_instance + str_prop->GetOffset_ForUFunction();
			//find nearest string
			int32 value = FMath::RoundToInt(label_values[ivalue]);
			const FString label_string = label_cache.GetString(iprop, value).ToString();
			//apply
			str_prop->SetPropertyValue(pdata, *label_string);
			ivalue++;
		}
		else if (FBoolProperty* bool_prop = CastField<FBoolProperty>(prop))
		{
			uint8* pdata = out_struct_instance + bool_prop->GetOffset_ForUFunction();
			//float/double
			float value = label_values[ivalue];
			bool_prop->SetPropertyValue(pdata, value > 0.5f);
			ivalue++;
		}
		else
		{
			UE_LOG(LogInteractML, Warning, TEXT("Unable to recreate unsupported type '%s' in field '%s' of label structure %s'"), *prop->GetClass()->GetName(), *prop->GetName(), *GetName());
		}

		prop = prop->Next;
		iprop++;
	}
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

