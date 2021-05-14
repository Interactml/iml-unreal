//
//  regressionEmbindings.cpp
//  RapidLib
//
//  Created by mzed on 26/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#ifndef regressionEmbindings_h
#define regressionEmbindings_h

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(regression_module) {
  class_<regressionTemplate<double>, base<modelSet<double> > >("RegressionCpp") //name change so that I can wrap it in Javascript. -mz
    .constructor()
    .constructor< std::vector<trainingExampleTemplate<double> > >()
    .constructor<int, int>()
    .function("train", &regressionTemplate<double>::train)
    .function("getNumHiddenLayers", &regressionTemplate<double>::getNumHiddenLayers)
    .function("setNumHiddenLayers", &regressionTemplate<double>::setNumHiddenLayers)
    .function("getNumEpochs", &regressionTemplate<double>::getNumEpochs)
    .function("setNumEpochs", &regressionTemplate<double>::setNumEpochs)
    .function("getNumHiddenNodes", &regressionTemplate<double>::getNumHiddenNodes)
    .function("setNumHiddenNodes", &regressionTemplate<double>::setNumHiddenNodes)
    ;

};

#endif
