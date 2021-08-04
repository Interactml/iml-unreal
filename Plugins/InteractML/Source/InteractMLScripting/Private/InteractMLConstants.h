//----
// InteractML - University Of Arts London
//----

#pragma once

//module
#include "CoreMinimal.h"

//unreal


// general interactml constants
namespace InteractMLConstants
{
	// node base colour
	// NOTE: hard to get clear as heavily overlaid/modified with white
	static FColor NodeTitleColour(128, 100, 255); //IML Purple-ish

	// category title for grouping iml nodes/etc in menus
	static FText NodeMenuCategory = NSLOCTEXT("InteractML", "NodeMenuCategory", "InteractML");

	// name for common actor pin of nodes
	static FName BlueprintNodeActorPinName("Actor");
	
	// tooltip for common actor pin of nodes
	static FText BlueprintNodeActorPinTooltip = NSLOCTEXT("InteractML", "BlueprintNodeActorPinTooltip", "Interact ML nodes need an actor to provide context in which they operate.\nTypically this would be actor the graph is attached to (i.e. 'Self').");
}