//
//  knnEmbindings.h
//  RapidLib
//
//  Created by mzed on 05/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#ifndef knnEmbindings_h
#define knnEmbindings_h

#include <vector>
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(stl_wrappers) {
  register_vector<int>("VectorInt");
  register_vector<double>("VectorDouble");
  register_vector<std::vector<double>>("VectorVectorDouble");

  register_vector<trainingExampleTemplate<double>>("TrainingSet");
  register_vector<trainingSeriesTemplate<double>>("TrainingSeriesSet");

  value_object<trainingExampleTemplate<double>>("trainingExample")
    .field("input", &trainingExampleTemplate<double>::input)
    .field("output", &trainingExampleTemplate<double>::output)
    ;

  value_object<trainingSeriesTemplate<double>>("trainingSeries")
    .field("input", &trainingSeriesTemplate<double>::input)
    .field("label", &trainingSeriesTemplate<double>::label)
    ;
}


EMSCRIPTEN_BINDINGS(knn_module) {
  class_<knnClassification<double>>("KnnClassification")
    .constructor<int, std::vector<int>, std::vector<trainingExampleTemplate<double>>, int>()
    .function("addNeighbour", &knnClassification<double>::addNeighbour)
    .function("run", &knnClassification<double>::run)
    ;
};

#endif
