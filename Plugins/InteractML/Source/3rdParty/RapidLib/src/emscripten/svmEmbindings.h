//
//  svmEmbindings.cpp
//  RapidLib
//
//  Created by mzed on 25/08/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//


#ifndef svmEmbindings_h
#define svmEmbindings_h

#include <vector>
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(svm_module) {
  class_<svmClassification<double>>("svmClassificationCPP")
    .constructor<int>()
    .function("train", &svmClassification<double>::train)
    .function("run", &svmClassification<double>::run)
    ;
};

#endif
