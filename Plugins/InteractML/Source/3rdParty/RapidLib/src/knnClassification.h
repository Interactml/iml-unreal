//
//  knnClassification.h
//  RapidLib
//
//  Created by mzed on 05/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#ifndef knnClassification_h
#define knnClassification_h

#include <vector>
#include "baseModel.h"

#ifndef EMSCRIPTEN
#include "../dependencies/json/json.h"
#endif

/** Class for implementing a knn classifier */
template<typename T>
class knnClassification final : public baseModel<T> {
    
public:
    /** Constructor that takes training examples in
     * @param number of inputs expected in the training and input vectors
     * @param vector of input numbers to be fed into the classifer.
     * @param vector of training examples
     * @param how many near neighbours to evaluate
     */
    knnClassification(const int &num_inputs,
                      const std::vector<int> &which_inputs,
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
    
    /** Reset the model to its empty state. */
    void reset() override;
    
    /** Find out how many inputs the model expects
     * @return Integer number of intpus
     */
    int getNumInputs() const override;
    
    /** Find out which inputs in a vector will be used
     * @return Vector of ints, specifying input indices.
     */
    std::vector<int> getWhichInputs() const override;
    
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
    std::vector<int> whichInputs;
    std::vector<trainingExampleTemplate<T>> neighbours;
    int desiredK; //K that user asked for might be limited but number of examples
    int currentK; //K minimum of desiredK or neighbours.size()
    inline void updateK();
    std::pair<int, T>* nearestNeighbours;
};

#endif

