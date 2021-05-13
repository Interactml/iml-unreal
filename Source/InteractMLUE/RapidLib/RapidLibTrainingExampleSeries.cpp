#include "RapidLibTrainingExampleSeries.h"


InteractML::FRapidLibTrainingExampleSeries::FRapidLibTrainingExampleSeries(TArray<TArray<double>> series, std::string labelSeries)
{
	if (series.Num() > 0) {
		exampleSeries = TArray<TArray<double>>(series);
	}
	else
	{
		exampleSeries = TArray<TArray<double>>();
	}

	if (labelSeries.size() > 0) {
		label = labelSeries;
	}
	else {
		label = "";
	}
}
