//
//  modelSetEmbindings.h
//  RapidLib
//
//  Created by mzed on 04/10/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#ifndef modelSetEmbindings_h
#define modelSetEmbindings_h

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(modelSet_module) {
  class_<modelSet<double> >("ModelSetCpp") //name change so that I can wrap it in Javascript. -mz
    .constructor()
    .function("train", &modelSet<double>::train)
    .function("reset", &modelSet<double>::reset)
    .function("run", &modelSet<double>::run)
    ;
  
};

#endif
