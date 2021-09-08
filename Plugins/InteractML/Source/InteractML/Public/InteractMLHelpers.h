//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//module
#include "CoreMinimal.h"

//unreal


// So, we have bool inputs to nodes that control things like Record, Train, Reset.
// These are often set to true for multiple frames and in order for us to only 
// perform the relevant action on a transition from false->true or true->false we
// need to track the previous values.  This means storing state somewhere, but
// blueprint nodes are state-less function graphs (largely) and so we have to store
// this in the object being acted on, i.e. the Training Set or Model.
// This struct is a little helper that does this, but it also does a little validation
// of which node is performing the action and warns when more than one is trying
// to invoke the action at once.
// Example of use:
//  if(target_object->DoSomething_interlock.Triggered(DoSomething_bool, node_id))
//  {
//    if(DoSomething_bool)
//        //just gone true
//    else
//        //just gone false
//  }
struct FNodeActionInterlock
{
private:
	FString NodeID;
	
public:
	// has this action triggered? i.e. state transitioned
	bool Triggered(bool action_state, FString node_id);

	// query action state
	bool Active() const { return !NodeID.IsEmpty(); }
};

