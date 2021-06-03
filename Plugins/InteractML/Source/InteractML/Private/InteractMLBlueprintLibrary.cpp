//----
// InteractML - University Of Arts London
//----

#include "InteractMLBlueprintLibrary.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLContext.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES


//////////////////////////////////// HELPERS ////////////////////////////////////

// ML operations going on in a blueprint need somewhere to store state both as control passes between nodes
// and between evaluation frames. This provides access to some such place, by adding a UInteractMLContext object
// to the actor on demand and returning an existing one if already added.  This is a transient object so won't be
// persisted along with the actor.
//
static UInteractMLContext* GetMLContext(AActor* Actor)
{
	//look for existing
	UInteractMLContext* context = Cast<UInteractMLContext>( Actor->GetComponentByClass(UInteractMLContext::StaticClass()) );

	//create/add on demand
	if (!context)
	{
		context = NewObject<UInteractMLContext>( Actor, NAME_None, RF_Transient | RF_DuplicateTransient );
		Actor->AddOwnedComponent( context );
		context->RegisterComponent();
	}

	return context;
}


//////////////////////////////// BLUEPRINT LIBRARY /////////////////////////////////


// Parameter Collection custom node: call to obtain a parameter object to accumulate into
//
FInteractMLParameters UInteractMLBlueprintLibrary::GetParameters(AActor* Actor, UInteractMLContext::TGraphNodeID NodeID)
{
	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );

	//get a parameter collection for this node to use
	TSharedPtr<FInteractMLParameterCollection> parameters = Context->GetParameters( NodeID );
	
	//prep for (re)accumulation
	parameters->Reset();

	return FInteractMLParameters(parameters);
}

// Parameter Collection custom node: call to add a parameter
//
void UInteractMLBlueprintLibrary::AddIntegerParameter(FInteractMLParameters Parameters, int Value)
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddFloatParameter( FInteractMLParameters Parameters, float Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddBooleanParameter( FInteractMLParameters Parameters, bool Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddVector2Parameter( FInteractMLParameters Parameters, FVector2D Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddVector3Parameter( FInteractMLParameters Parameters, FVector Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddQuaternionParameter( FInteractMLParameters Parameters, FQuat Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddColourParameter( FInteractMLParameters Parameters, FLinearColor Value )
{
	Parameters.Ptr->Add(Value);
}


//////////////////////////////// TEMP/TEST/DEBUG /////////////////////////////////

// dump parameter collection output
//
void UInteractMLBlueprintLibrary::DebugParameterCollection(FInteractMLParameters Parameters)
{
	FInteractMLParameterCollection* params = Parameters.Ptr.Get();
	if (params)
	{
		FString txt;
		for (int i = 0; i < params->Values.Num(); i++)
		{
			if (i > 0)
			{
				txt.Append(TEXT(", "));
			}
			float val = params->Values[i];
			txt.Appendf( TEXT("%.00f"), val );
		}
		UE_LOG(LogInteractML, Display, TEXT("Parameters (%i): %s"), params->Values.Num(), *txt);
	}
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

