//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//module
#include "CoreMinimal.h"

//unreal


// general interactml constants
//
namespace InteractMLConstants
{
	// node base colour
	// NOTE: hard to get clear as heavily overlaid/modified with white for some reason
	//static FColor NodeTitleColour(128, 100, 255); //IML Purple-ish - too purple
	static FColor NodeTitleColour(142, 137, 246); //IML Bright-purple - ???
	//static FColor NodeTitleColour(59, 58, 91); //IML Dark-purple - too dark

	// category title for grouping iml nodes/etc in menus
	static FText NodeMenuCategory = NSLOCTEXT("InteractML", "NodeMenuCategory", "InteractML");

	// name for common actor pin of nodes
	static FName BlueprintNodeActorPinName("Actor");
	
	// tooltip for common actor pin of nodes
	static FText BlueprintNodeActorPinTooltip = NSLOCTEXT("InteractML", "BlueprintNodeActorPinTooltip", "Interact ML nodes need an actor to provide context in which they operate.\nTypically this would be actor the graph is attached to (i.e. 'Self').");
}