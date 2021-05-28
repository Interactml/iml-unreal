//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//module
#include "InteractMLParameters.generated.h"

//general declarations


// wrapper for passing around parameters by value without copying
//
USTRUCT(BlueprintInternalUseOnly)
struct INTERACTML_API FInteractMLParametersPtr
{
	GENERATED_USTRUCT_BODY()
	TSharedPtr<struct FInteractMLParameters> Ptr;

	//ctors
	FInteractMLParametersPtr(){}
	FInteractMLParametersPtr(TSharedPtr<struct FInteractMLParameters> ptr) : Ptr(ptr) {}
};

// InteractML parameter state snapshot, single sample/capture/frame of parameter values
// List of parameters with associated name/type/value
// In-memory use only for passing around in node graph
// NOTE: Not a UObject or USTRUCT, just a plain struct
// NOTE: pass around via FInteractMLParametersPtr wrapper
//
struct INTERACTML_API FInteractMLParameters
{
	//type information
	//TArray<FString>  Names;
	//TArray<FName>    Types;

	//parameter value snapshot
	TArray<float>    Values;

	//accumulation cursor, i.e. which value we're adding next
	int              Cursor;

	
	//---- setup ----

	// completely clear out state
	void Clear();

	// start accumulation from first parameter again
	void Reset();

	//accumulation
	void Add(bool value);
	void Add(float value);
	void Add(int value);
	void Add(FVector value);
	void Add(FQuat value);
	void Add(FLinearColor value);
	void Add(FColor value);
	

private:

};
