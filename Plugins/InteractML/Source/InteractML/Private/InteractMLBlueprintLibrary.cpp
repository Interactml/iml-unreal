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


// ML operations going on in a blueprint need somewhere to store state both as control passes between nodes
// and between evaluation frames. This provides access to some such place, by adding a UInteractMLContext object
// to the actor on demand and returning an existing one if already added.  This is a transient object so won't be
// persisted along with the actor.
//
UInteractMLContext* UInteractMLBlueprintLibrary::GetMLContext(AActor* Actor)
{
	//look for existing
	UInteractMLContext* context = Cast<UInteractMLContext>( Actor->GetComponentByClass(UInteractMLContext::StaticClass()) );

	//create/add on demand
	if (!context)
	{
		context = NewObject<UInteractMLContext>( Actor, NAME_None, RF_Transient | RF_DuplicateTransient );
		context->InitializeComponent();
		Actor->AddOwnedComponent( context );
	}

	return context;
}

// Parameter Collection custom node: call to obtain a parameter object to accumulate into
//
FInteractMLParametersPtr UInteractMLBlueprintLibrary::GetParameters(UInteractMLContext* Context, UInteractMLContext::TGraphNodeID NodeID)
{
	TSharedPtr<FInteractMLParameters> parameters = Context->GetParameters(NodeID);
	
	//prep for (re)accumulation
	parameters->Reset();

	return FInteractMLParametersPtr(parameters);
}

// Parameter Collection custom node: call to add a parameter
//
void UInteractMLBlueprintLibrary::AddFloatParameter(FInteractMLParametersPtr Parameters, float Value)
{
	Parameters.Ptr->Add(Value);
}

// EPILOGUE
#undef LOCTEXT_NAMESPACE

