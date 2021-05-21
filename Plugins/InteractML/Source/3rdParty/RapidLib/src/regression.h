/**
 * @file regression.h
 * RapidLib
 *
 * @author Michael Zbsyzynski
 * @date 26 Sep 2016
 * @copyright Copyright © 2016 Goldsmiths. All rights reserved.
 */

#ifndef regression_h
#define regression_h

#include <vector>
#include "modelSet.h"

/*! Class for implementing a set of regression models.
 *
 * This doesn't do anything modelSet can't do. But, it's simpler and more like wekinator.
 */

template<typename T>
class regressionTemplate : public modelSet<T> {
public:
    /** with no arguments, just make an empty vector */
    regressionTemplate();
    /** create based on training set inputs and outputs */
    regressionTemplate(const std::vector<trainingExampleTemplate<T> > &trainingSet);
    /** create with proper models, but not trained */
    regressionTemplate(const int &numInputs, const int &numOutputs);
    
    /** destructor */
    ~regressionTemplate() {};
    
    /** Train on a specified set, causes creation if not created */
    bool train(const std::vector<trainingExampleTemplate<T> > &trainingSet) override;
    
    /** Check how many training epochs each model will run. This feature is temporary, and will be replaced by a different design. */
    std::vector<int> getNumEpochs() const;
    
    /** Call before train, to set the number of training epochs */
    void setNumEpochs(const int &epochs);
    
    /** Check how many hidden layers are in each model. This feature is temporary, and will be replaced by a different design. */
    std::vector<int> getNumHiddenLayers() const;
    
    /** Set how many hidden layers are in all models. This feature is temporary, and will be replaced by a different design. */
    void setNumHiddenLayers(const int &num_hidden_layers);
    
    /** Check how many hidden nodes are in each model. This feature is temporary, and will be replaced by a different design. */
    std::vector<int> getNumHiddenNodes() const;
    
    /** Set how many hidden layers are in all models. This feature is temporary, and will be replaced by a different design. */
    void setNumHiddenNodes(const int &num_hidden_nodes);
    
private:
    int numHiddenLayers; //Temporary -- this should be part of the nn class. -mz
    int numEpochs; //Temporary -- also should be part of nn only. -mz
    int numHiddenNodes; //Temporary -- also should be part of nn only. -mz
};

//This is here so the old API still works
using regression = regressionTemplate<double>;
using regressionFloat = regressionTemplate<float>;

#endif
