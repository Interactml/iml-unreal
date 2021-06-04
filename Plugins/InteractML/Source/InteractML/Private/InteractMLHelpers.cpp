//----
// InteractML - University Of Arts London
//----

#include "InteractMLHelpers.h"

//unreal

//module
#include "InteractML.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES


// has this action triggered? i.e. state transitioned
//
bool FNodeActionInterlock::Triggered(bool action_state, FString node_id)
{
	bool is_active = !NodeID.IsEmpty();
	bool want_active = action_state;
	if (want_active != is_active)
	{
		//transition
		if (want_active)
		{
			//just became active
			NodeID = node_id;
		}
		else if(node_id==NodeID)	//validate same node
		{
			//just become inactive
			NodeID.Empty();
		}
		
		//transitioned, trigger
		return true;
	}
	
	//validate same node holding it active
	if(want_active && node_id!=NodeID)
	{
		UE_LOG(LogInteractML, Warning, TEXT("Multiple nodes are trying to activate functionality of a training set or model at the same time."));
	}
	
	//stable state, no action required
	return false;
}

// EPILOGUE
#undef LOCTEXT_NAMESPACE

