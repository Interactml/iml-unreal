//
//  modelSet.h
//  RapidLib
//
//  Created by mzed on 26/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#pragma once

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
    
    /** Generate an output value from a single input vector.
    *
    * Will return an error if training in progress.
    * 
    * @param vector A standard vector of type T that is the input for classification or regression.
    * @return vector A vector type T that are the predictions for each model in the set.
    */
    std::vector<T> run(const std::vector<T> &inputVector);
    
protected:
    std::vector<baseModel<T>*> myModelSet;
    int numInputs;
    std::vector<std::string> inputNames;
    int numOutputs;
    bool isTraining; //This is true while the models are training, and will block running
    bool isTrained;
    void threadTrain(std::size_t i, const std::vector<trainingExampleTemplate<T> >& training_set);

#ifndef EMSCRIPTEN //The javascript code will do its own JSON parsing
public:

    /** Get a JSON representation of the model
    * 
    * @return Styled string JSON representation
    */
    std::string getJSON();

    /** Write a JSON model description to specified file path 
    *
    * @param file path
    * 
    */
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
