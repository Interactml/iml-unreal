/**
 * @file svmClassification.cpp
 * RapidLib
 *
 * @author Michael Zbyszynski
 * @date 23 Feb 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#include <iostream>
#include "svmClassification.h"
#ifdef EMSCRIPTEN
#include "emscripten/svmEmbindings.h"
#endif

template<typename T>
svmClassification<T>::svmClassification(
                                        KernelType kernelType,
                                        SVMType svmType,
                                        bool useScaling,
                                        bool useNullRejection,
                                        bool useAutoGamma,
                                        float gamma,
                                        unsigned int degree,
                                        float coef0,
                                        float nu,
                                        float C,
                                        bool useCrossValidation,
                                        unsigned int kFoldValue
                                        )
{
    
    //Setup the default SVM parameters
    model = NULL;
    problem.l = 0;
    problem.x = NULL;
    problem.y = NULL;
    trained = false;
    problemSet = false;
    param.svm_type = C_SVC;
    param.kernel_type = POLY_KERNEL; //LINEAR_KERNEL;
    param.degree = 3;
    param.gamma = 0;
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    
    //These are from GTK?
    /*
     this->useScaling = false;
     this->useCrossValidation = false;
     this->useNullRejection = false;
     this->useAutoGamma = true;
     classificationThreshold = 0.5;
     crossValidationResult = 0;
     
     classifierMode = STANDARD_CLASSIFIER_MODE;
     */
    init(kernelType,svmType,useScaling,useNullRejection,useAutoGamma,gamma,degree,coef0,nu,C,useCrossValidation,kFoldValue);
}

template<typename T>
svmClassification<T>::svmClassification(int num_inputs) {
    
    numInputs = num_inputs;
    
    //Setup the default SVM parameters
    model = NULL;
    param.weight_label = NULL;
    param.weight = NULL;
    problem.l = 0;
    problem.x = NULL;
    problem.y = NULL;
    trained = false;
    problemSet = false;
    param.svm_type = C_SVC;
    param.kernel_type = POLY_KERNEL; //LINEAR_KERNEL;
    param.degree = 3;
    param.gamma = 0;
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    
}

template<typename T>
svmClassification<T>::~svmClassification() {
    
}

template<typename T>
void svmClassification<T>::reset() {
    //TODO: implement me
}

template<typename T>
bool svmClassification<T>::init(
                                KernelType kernelType,
                                SVMType svmType,
                                bool useScaling,
                                bool useNullRejection,
                                bool useAutoGamma,
                                float gamma,
                                unsigned int degree,
                                float coef0,
                                float nu,
                                float C,
                                bool useCrossValidation,
                                unsigned int kFoldValue
                                ){
    
    /*
     //Clear any previous models or problems
     clear();
     
     //Validate the kernerlType
     if( !validateKernelType(kernelType) ){
     errorLog << __GRT_LOG__ << " Unknown kernelType!\n";
     return false;
     }
     
     if( !validateSVMType(svmType) ){
     errorLog << __GRT_LOG__ << " Unknown kernelType!\n";
     return false;
     }
     */
    
    param.svm_type = (int)svmType;
    param.kernel_type = (int)kernelType;
    param.degree = (int)degree;
    param.gamma = gamma;
    param.coef0 = coef0;
    param.nu = nu;
    param.cache_size = 100;
    param.C = C;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    /*
     this->useScaling = useScaling;
     this->useCrossValidation = useCrossValidation;
     this->useNullRejection = useNullRejection;
     this->useAutoGamma = useAutoGamma;
     classificationThreshold = 0.5;
     crossValidationResult = 0;
     */
    
    return true;
}

template<typename T>
void svmClassification<T>::train(const std::vector<trainingExampleTemplate<T> > &trainingSet) {
    //TODO: should be scaling data -1 to 1
    //Get normalization parameters
    std::vector<T> inMax = trainingSet[0].input;
    std::vector<T> inMin = trainingSet[0].input;
    for (int ti = 1; ti < (int) trainingSet.size(); ++ti) {
        for (int i = 0; i < numInputs; ++i) {
            if (trainingSet[ti].input[i] > inMax[i]) {
                inMax[i] = trainingSet[ti].input[i];
            }
            if (trainingSet[ti].input[i] < inMin[i]) {
                inMin[i] = trainingSet[ti].input[i];
            }
        }
    }
    inRanges.clear();
    inBases.clear();
    for (int i = 0; i < numInputs; ++i) {
        inRanges.push_back((inMax[i] - inMin[i]) * 0.5);
        inBases.push_back((inMax[i] + inMin[i]) * 0.5);
    }
    for (int i = 0; i < inRanges.size(); ++i) {
        if (inRanges[i] == 0.) {
            inRanges[i] = 1.0; //Prevent divide by zero later.
        }
    }
    
    //initialize problem
    problem.l = 0;
    problem.x = NULL;
    problem.y = NULL;
    
    //SVM problem has:
    int numberOfExamples = int(trainingSet.size());
    int numberOfFeatures = int(trainingSet[0].input.size());
    problem.l = numberOfExamples;
    problem.x = new LIBSVM::svm_node*[numberOfExamples];
    problem.y = new double[numberOfExamples];
    for (int i = 0; i < numberOfExamples; i++) {
        problem.y[i] = trainingSet[i].output[0]; //model set makes this a one item list
        problem.x[i] = new LIBSVM::svm_node[numberOfFeatures + 1]; //dummy node at the end of array
        for (int j = 0; j < numberOfFeatures; j++) {
            // x = svn_nodes[]  == index and value pairs
            problem.x[i][j].index = j + 1;
            problem.x[i][j].value = ((trainingSet[i].input[j] - inBases[j]) / inRanges[j]); //TODO: make normalization optional
        }
        problem.x[i][numberOfFeatures].index = -1; //Assign the final node value
        problem.x[i][numberOfFeatures].value = 0;
    }
    
    model = LIBSVM::svm_train(&problem, &param);
    trained = true;
};

template<typename T>
T svmClassification<T>::run(const std::vector<T> &inputVector) {
    if (trained) {
        double predictedClass = 0.;
        
        //Change to LIBSVM format
        LIBSVM::svm_node *inputNodes = NULL;
        inputNodes = new LIBSVM::svm_node[numInputs + 1];
        for (int i = 0; i < numInputs; i++) {
            inputNodes[i].index = i + 1;
            inputNodes[i].value = ((inputVector[i] - inBases[i]) / inRanges[i]); //TODO: make normalization optional
        }
        inputNodes[numInputs].index = -1;
        inputNodes[numInputs].value = 0;
        
        predictedClass = LIBSVM::svm_predict(model, inputNodes);
        delete[] inputNodes;
        return predictedClass;
    } else {
        return 0;
    }
}

template<typename T>
int svmClassification<T>::getNumInputs() const {
    return 0;
};

template<typename T>
std::vector<int> svmClassification<T>::getWhichInputs() const {
    std::vector<int> returnVec;
    return returnVec;
};

#ifndef EMSCRIPTEN
template<typename T>
void svmClassification<T>::getJSONDescription(Json::Value &currentModel){
    
};
#endif

//explicit instantiation
template class svmClassification<double>;
template class svmClassification<float>;
