//
//  seriesClassification.h
//  RapidLib
//
//  Created by mzed on 13/06/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//


#ifndef seriesClassificationEmbindings_h
#define seriesClassificationEmbindings_h

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(seriesClassification_module) {
  class_<seriesClassificationTemplate<double> >("SeriesClassificationCpp") //name change so that I can wrap it in Javascript. -mz
    .constructor()
    .function("reset", &seriesClassificationTemplate<double>::reset)
    .function("train", &seriesClassificationTemplate<double>::train)
    .function("run", select_overload<std::string(const std::vector<std::vector<double>>&)>(&seriesClassificationTemplate<double>::run))
    .function("runLabel", select_overload<double(const std::vector<std::vector<double>>&, std::string)>(&seriesClassificationTemplate<double>::run))
    .function("getCosts", &seriesClassificationTemplate<double>::getCosts)
    ;
};

#endif
