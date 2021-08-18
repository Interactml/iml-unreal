//
//  classification.h
//  RapidLib
//
//  Created by mzed on 26/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#pragma once

#include <vector>
#include "modelSet.h"

/*! Class for implementing a set of classification models.
 *
 * This doesn't do anything modelSet can't do. But, it's simpler and more like wekinator.
 * 
 */

template<typename T>
class classificationTemplate final : public modelSet<T>
{
public:
    enum classificationTypes { knn, svm };
    
    /** Create with no arguments
    *
    * Default classifier is kNN.
    */
    classificationTemplate();
    
    /** Specify classification type 
    * 
    * @param Enum Classification type: knn or svm
    */
    classificationTemplate(classificationTypes classificationType);
    
    /** create based on training set inputs and outputs */
    classificationTemplate(const std::vector<trainingExampleTemplate<T> > &trainingSet);

    /** create with proper models, but not trained */
    classificationTemplate(const int &numInputs, const int &numOutputs);
    
    /** destructor */
    ~classificationTemplate() {}
    
    /** Train on a specified set, causes creation if not created 
    *
    * @param vector Vector of training examples, type T
    * @return bool Successful training
    */
    bool train(const std::vector<trainingExampleTemplate<T> > &trainingSet) override;
    
    /** Check the K values for each model. 
    * 
    * This feature is temporary, and will be replaced by a different design. 
    * @return vector K for every model
    */
    std::vector<int> getK();

    /** Set the K values for each model. 
    * This feature is temporary, and will be replaced by a different design. 
    *
    * @param int which model to set
    * @param int k value for that model
    */
    void setK(const int whichModel, const int newK);
     
private:
    classificationTypes classificationType;
};

namespace rapidLib
{
    //This is here so that the old API still works as expected.
    using classification = classificationTemplate<double>;
    using classificationFloat = classificationTemplate<float>;
}
