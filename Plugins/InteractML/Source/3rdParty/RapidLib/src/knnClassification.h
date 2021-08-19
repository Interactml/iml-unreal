//
//  knnClassification.h
//  RapidLib
//
//  Created by mzed on 05/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#pragma once

#include <vector>
#include "baseModel.h"

#ifndef EMSCRIPTEN
#include "../dependencies/json/json.h"
#endif

/** Class for implementing a knn classifier */
template<typename T>
class knnClassification final : public baseModel<T> 
{    
public:
    /** Constructor that takes training examples in
     * @param int Number of inputs expected in the training and input vectors
     * @param vector of input numbers to be fed into the classifer.
     * @param vector of training examples
     * @param int how many near neighbours to evaluate
     */
    knnClassification(const int &num_inputs,
                      const std::vector<size_t> &which_inputs,
                      const std::vector<trainingExampleTemplate<T> > &trainingSet,
                      const int k);
    ~knnClassification();
    
    /** add another example to the existing training set
     * @param class number of example
     * @param feature vector of example
     */
    void addNeighbour(const int &classNum, const std::vector<T> &features);
    
    /** Generate an output value from a single input vector.
     * @param A standard vector of type T to be evaluated.
     * @return A single value of type T: the nearest class as determined by k-nearest neighbor.
     */
    T run(const std::vector<T> &inputVector) override;
    
    /** Fill the model with a vector of examples.
     *
     * @param The training set is a vector of training examples that contain both a vector of input values and a value specifying desired output class.
     *
     */
    void train(const std::vector<trainingExampleTemplate<T> > &trainingSet) override;

    /** Fill the model with a vector of examples. Use this when the model is part of a modelSet.
     *
     * @param The training set is a vector of training examples that contain both a vector of input values and a value specifying desired output class.
     *
     */
    void train(const std::vector<trainingExampleTemplate<T> >& trainingSet, const std::size_t whichOutput) override;
    
    /** Reset the model to its empty state. */
    void reset() override;
    
    /** Find out how many inputs the model expects
     * @return Integer number of intpus
     */
    size_t getNumInputs() const override;
    
    /** Find out which inputs in a vector will be used
     * @return Vector of ints, specifying input indices.
     */
    std::vector<size_t> getWhichInputs() const override;
    
    /** Get the number of nearest neighbours used by the kNN algorithm. */
    int getK() const;
    /** Change the number of nearest neighbours used by the kNN algorithm.
     * @param new value for k
     */
    void setK(int newK);
    
#ifndef EMSCRIPTEN
    /** Populate a JSON value with a description of the current model
     * @param A JSON value to be populated
     */
    void getJSONDescription(Json::Value &currentModel) override;
#endif
    
private:
    int numInputs;
    std::vector<size_t> whichInputs;
    std::size_t whichOutput;
    std::vector<trainingExampleTemplate<T>> neighbours;
    int desiredK; //K that user asked for might be limited but number of examples
    int currentK; //K minimum of desiredK or neighbours.size()
    inline void updateK();
};
