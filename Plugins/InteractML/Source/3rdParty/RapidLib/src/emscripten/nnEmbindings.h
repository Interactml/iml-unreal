//
//  nnEmbindings.h
//  RapidLib
//
//  Created by mzed on 22/08/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#ifndef nnEmbindings_h
#define nnEmbindings_h

#include <vector>
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(nn_module) {
   class_<neuralNetwork<double>>("NeuralNetwork")
     .constructor<int, std::vector<int>, int, int>()
     .constructor<int, std::vector<int>, int, int, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>, double, double>()
     .function("run", &neuralNetwork<double>::run)
     .function("train", &neuralNetwork<double>::train)
     ;

};
#endif
