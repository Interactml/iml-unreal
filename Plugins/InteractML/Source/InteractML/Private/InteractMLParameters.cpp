//----
// InteractML - University Of Arts London
//----

#include "InteractMLParameters.h"

//unreal

//module


// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES



// completely clear out state
void FInteractMLParameterCollection::Clear()
{
	Values.Empty();
	Cursor = 0;
}

// start accumulation from first parameter again
void FInteractMLParameterCollection::Reset()
{
	Cursor = 0;
}

//accumulation
void FInteractMLParameterCollection::Add(bool value)
{
	AccumulateValue( value );
}
void FInteractMLParameterCollection::Add(float value)
{
	AccumulateValue(value);
}
void FInteractMLParameterCollection::Add(int value)
{
	AccumulateValue((float)value);
}
void FInteractMLParameterCollection::Add(FVector2D value)
{
	AccumulateValue(value.X);
	AccumulateValue(value.Y);
}
void FInteractMLParameterCollection::Add(FVector value)
{
	AccumulateValue(value.X);
	AccumulateValue(value.Y);
	AccumulateValue(value.Z);
}
void FInteractMLParameterCollection::Add(FQuat value)
{
	AccumulateValue(value.X);
	AccumulateValue(value.Y);
	AccumulateValue(value.Z);
	AccumulateValue(value.W);
}
void FInteractMLParameterCollection::Add(FLinearColor value)
{
	AccumulateValue(value.R);
	AccumulateValue(value.G);
	AccumulateValue(value.B);
}
void FInteractMLParameterCollection::Add(FColor value)
{
	Add(value.ReinterpretAsLinear());
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

