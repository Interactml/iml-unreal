//----
// InteractML - University Of Arts London
//----

#pragma once

//unreal
#include "CoreMinimal.h"

//module


//general declarations


// InteractML parameter state snapshot
// List of parameters with associated name/type/value
// In-memory use only for passing around in node graph
// NOTE: pass around using FInteractMLParameters::Ptr pr FInteractMLParameters::Ref
//
class INTERACTML_API FInteractMLParameters : public TSharedFromThis<FInteractMLParameters>
{


public:
	typedef TSharedPtr<FInteractMLParameters> Ptr;
	typedef TSharedRef<FInteractMLParameters> Ref;
	
	//access



private:

};
