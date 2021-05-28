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
void FInteractMLParameters::Clear()
{
	Values.Empty();
	Cursor = 0;
}

// start accumulation from first parameter again
void FInteractMLParameters::Reset()
{
	Cursor = 0;
}

//accumulation
void FInteractMLParameters::Add(bool value)
{
	Values.Add(value?1.0f:0.0f);
}
void FInteractMLParameters::Add(float value)
{
	Values.Add(value);
}
void FInteractMLParameters::Add(int value)
{
	Values.Add((float)value);
}
void FInteractMLParameters::Add(FVector value)
{
	Values.Add(value.X);
	Values.Add(value.Y);
	Values.Add(value.Z);
}
void FInteractMLParameters::Add(FQuat value)
{
	Values.Add(value.X);
	Values.Add(value.Y);
	Values.Add(value.Z);
	Values.Add(value.W);
}
void FInteractMLParameters::Add(FLinearColor value)
{
	Values.Add(value.R);
	Values.Add(value.G);
	Values.Add(value.B);
}
void FInteractMLParameters::Add(FColor value)
{
	Add(value.ReinterpretAsLinear());
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

