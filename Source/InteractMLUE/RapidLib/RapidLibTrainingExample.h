#pragma once
#ifndef RapidLibTrainingExample_h
#define RapidLibTrainingExample_h

#include "CoreMinimal.h"
#include <vector>
#include <string>

namespace InteractML {

	struct FRapidLibTrainingExample {

	public:
		FRapidLibTrainingExample(TArray<double> inputs, TArray<double> outputs);
		TArray<double> input;
		TArray<double> output;
	};

}


/*struct RapidLibTrainingExampleSeries {

public:
	std::vector<T> input;
	std::vector<T> output;


};*/
#endif 

