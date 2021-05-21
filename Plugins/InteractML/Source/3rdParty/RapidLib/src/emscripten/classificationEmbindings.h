//
//  classificationEmbindings.cpp
//  RapidLib
//
//  Created by mzed on 27/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//


#ifndef classificationEmbindings_h
#define classificationEmbindings_h

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(classification_module) {
  class_<classificationTemplate<double>, base<modelSet<double> > >("ClassificationCpp") //name change so that I can wrap it in Javascript. -mz
    .constructor()
    .constructor<classificationTemplate<double>::classificationTypes>()
    //    .constructor< std::vector<trainingExample> >()
    .constructor<int, int>()
    .function("train", &classificationTemplate<double>::train)
    .function("getK", &classificationTemplate<double>::getK)
    .function("setK", &classificationTemplate<double>::setK)
    ;
  enum_<classificationTemplate<double>::classificationTypes>("ClassificationTypes")
    .value("KNN", classificationTemplate<double>::knn)
    .value("SVM", classificationTemplate<double>::svm)
    ;

};

#endif
