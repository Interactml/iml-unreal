//----
// InteractML - University Of Arts London
//----

//module
#include "BlueprintLibrary.h"
#include "InteractML.h"

//unreal

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"



///////////////////////////////// HELPERS //////////////////////////////////

// IML Test function, does nothing
//
UFUNCTION(BlueprintCallable, Category = "InteractML")
int UInteractMLBlueprintLibrary::DoNothing(int i, float f)
{
	return 9;
}















// EPILOGUE
#undef LOCTEXT_NAMESPACE


