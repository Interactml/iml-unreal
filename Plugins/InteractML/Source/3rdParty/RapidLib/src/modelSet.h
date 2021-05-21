//
//  modelSet.h
//  RapidLib
//
//  Created by mzed on 26/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#ifndef modelSet_h
#define modelSet_h

#include <vector>
#include "trainingExample.h"
#include "baseModel.h"
#include "neuralNetwork.h"
#include "knnClassification.h"
#include "svmClassification.h"
#ifndef EMSCRIPTEN
#include "../dependencies/json/json.h"
#endif

/** This class holds a set of models with the same or different algorithms. */
template<typename T>
class modelSet {
public:
    modelSet();
    virtual ~modelSet();
    /** Train on a specified set, causes creation if not created */
    virtual bool train(const std::vector<trainingExampleTemplate<T> > &trainingSet);
    /** reset to pre-training state */
    bool reset();
    /** run regression or classification for each model */
    std::vector<T> run(const std::vector<T> &inputVector);
    
protected:
    std::vector<baseModel<T>*> myModelSet;
    int numInputs;
    std::vector<std::string> inputNames;
    int numOutputs;
    bool created;

#ifndef EMSCRIPTEN //The javascript code will do its own JSON parsing
public:
    /** Get a JSON representation of the model in the form of a styled string */
    std::string getJSON();
    /** Write a JSON model description to specified file path */
    void writeJSON(const std::string &filepath);
    /** configure empty model with string. See getJSON() */
    bool putJSON(const std::string &jsonMessage);
    /** read a JSON file at file path and build a modelSet from it */
    bool readJSON(const std::string &filepath);
        
private:
    Json::Value parse2json();
    void json2modelSet(const Json::Value &root);

#endif
};

#endif
