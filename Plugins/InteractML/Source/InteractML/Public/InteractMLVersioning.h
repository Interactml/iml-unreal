//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#pragma once

//unreal
#include "CoreMinimal.h"
#include "Misc/EngineVersionComparison.h"

//utility
#define UE_VERSION_AT_LEAST(a,b,c) (!UE_VERSION_OLDER_THAN(a,b,c))

//ticker changes
#if UE_VERSION_AT_LEAST(5,0,0)
#define TICKER_HANDLE FTSTicker::FDelegateHandle
#define TICKER_TYPE FTSTicker
#else //older, pre-5.0
#define TICKER_HANDLE FDelegateHandle
#define TICKER_TYPE FTicker
extern bool IsValid( UObject* p );
extern bool IsValidChecked( UObject* p );
#endif

//style lookup changes
#if UE_VERSION_AT_LEAST(5,1,0)
# define INTERACTML_STYLE FAppStyle::Get()
#else
# define INTERACTML_STYLE FEditorStyle::Get()
#endif