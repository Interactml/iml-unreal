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
	Values.Add(value?1.0f:0.0f);
}
void FInteractMLParameterCollection::Add(float value)
{
	Values.Add(value);
}
void FInteractMLParameterCollection::Add(int value)
{
	Values.Add((float)value);
}
void FInteractMLParameterCollection::Add(FVector2D value)
{
	Values.Add(value.X);
	Values.Add(value.Y);
}
void FInteractMLParameterCollection::Add(FVector value)
{
	Values.Add(value.X);
	Values.Add(value.Y);
	Values.Add(value.Z);
}
void FInteractMLParameterCollection::Add(FQuat value)
{
	Values.Add(value.X);
	Values.Add(value.Y);
	Values.Add(value.Z);
	Values.Add(value.W);
}
void FInteractMLParameterCollection::Add(FLinearColor value)
{
	Values.Add(value.R);
	Values.Add(value.G);
	Values.Add(value.B);
}
void FInteractMLParameterCollection::Add(FColor value)
{
	Add(value.ReinterpretAsLinear());
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

