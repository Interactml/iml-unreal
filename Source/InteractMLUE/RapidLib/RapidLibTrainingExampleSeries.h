#pragma once
#ifndef RapidLibTrainingExampleSeries_h
#define RapidLibTrainingExampleSeries_h

#include "CoreMinimal.h"
#include <vector>
#include <string>

namespace InteractML {

	struct FRapidLibTrainingExampleSeries {

	public:
		FRapidLibTrainingExampleSeries(TArray<TArray<double>> series, std::string seriesLabel);
		TArray<TArray<double>> exampleSeries;
		std::string label; 

	};

}


#endif 
