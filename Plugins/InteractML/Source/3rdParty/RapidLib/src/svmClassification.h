/**
 * @file svmClassification.h
 * RapidLib
 *
 * @author Michael Zbyszynski
 * @date 23 Feb 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#pragma once

#include <vector>
#include "baseModel.h"
#include "../dependencies/libsvm/libsvm.h"

template<typename T>
class svmClassification final : public baseModel<T>
{
public:
    enum SVMType{ C_SVC = 0, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };
    enum KernelType{ LINEAR_KERNEL = 0, POLY_KERNEL, RBF_KERNEL, SIGMOID_KERNEL, PRECOMPUTED_KERNEL };
    
    /**
     Default constructor.
     
     Set the initial SVM settings, although these can be changed at any time using either init(...) function of the set... functions.
     
     @param kernelType: this sets the SVM kernelType. Options are LINEAR_KERNEL, POLY_KERNEL, RBF_KERNEL, SIGMOID_KERNEL, PRECOMPUTED_KERNEL. The default kernelType is kernelType=LINEAR_KERNEL
     @param svmType: this sets the SVM type. Options are C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR. The default svmType is svmType=C_SVC
     @param useScaling: sets if the training/prediction data will be scaled to the default range of [-1. 1.]. The SVM algorithm commonly achieves a better classification result if scaling is turned on. The default useScaling value is useScaling=true
     @param useNullRejection: sets if a predicted class will be rejected if the classes' probability is below the classificationThreshold. The default value is useNullRejection=false
     @param useAutoGamma: sets if the SVM gamma parameter will automatically be computed, if set to true then gamma will be set to (1.0/numFeatures), where numFeatures is the number of features in the training data. The default value is useAutoGamma=true
     @param gamma: sets the SVM gamma parameter. The default value is gamma=0.1
     @param degree: sets the SVM degree parameter. The default value is degree=3
     @param coef0: sets the SVM coef0 parameter. The default value is coef0=0
     @param nu: sets the SVM nu parameter. The default value is nu=0.5
     @param C: sets the SVM C parameter. The default value is C=1
     @param useCrossValidation: sets if the SVM model will be trained using cross validation. The default value is useCrossValidation=false
     @param kFoldValue: sets the number of folds that will be used for cross validation. The default value is kFoldValue=10
     */
    
    svmClassification(
                      KernelType kernelType = LINEAR_KERNEL,
                      SVMType svmType = C_SVC,
                      bool useScaling = true,
                      bool useNullRejection = false,
                      bool useAutoGamma = true,
                      float gamma = 0.1,
                      unsigned int degree = 3,
                      float coef0 = 0,
                      float nu = 0.5,
                      float C = 1,
                      bool useCrossValidation = false,
                      unsigned int kFoldValue = 10
                      );
    
    svmClassification(int numInputs);
    
    /** Destructor */
    ~svmClassification();
    
    /** Fill the model with a vector of examples.
     *
     * @param The training set is a vector of training examples that contain both a vector of input values and a double specifying desired output class.
     *
     */
    void train(const std::vector<trainingExampleTemplate<T> > &trainingSet) override;

    /** Fill the model with a vector of examples. Use this when part of a modelSet
     *
     * @param The training set is a vector of training examples that contain both a vector of input values and a double specifying desired output class.
     * @param The second argument specifies which output this model is using.
     *
     */
    void train(const std::vector<trainingExampleTemplate<T> >& trainingSet, const std::size_t whichOutput) override;
    
    /** Generate an output value from a single input vector.
     * @param A standard vector of doubles to be evaluated.
     * @return A single double: the nearest class as determined by k-nearest neighbor.
     */
    T run(const std::vector<T> &inputVector) override;
    
    void reset() override;
    
    /**
     This initializes the SVM settings and parameters.  Any previous model, settings, or problems will be cleared.
     
     @param kernelType: this sets the SVM kernelType. Options are LINEAR_KERNEL, POLY_KERNEL, RBF_KERNEL, SIGMOID_KERNEL, PRECOMPUTED_KERNEL
     @param UINT svmType: this sets the SVM type. Options are C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR
     @param useScaling: sets if the training/prediction data will be scaled to the default range of [-1. 1.]. The SVM algorithm commonly achieves a better classification result if scaling is turned on
     @param useNullRejection: sets if a predicted class will be rejected if the classes' probability is below the classificationThreshold
     @param useAutoGamma: sets if the SVM gamma parameter will automatically be computed, if set to true then gamma will be set to (1.0/numFeatures), where numFeatures is the number of features in the training data
     @param gamma: sets the SVM gamma parameter
     @param degree: sets the SVM degree parameter
     @param coef0: sets the SVM coef0 parameter
     @param nu: sets the SVM nu parameter
     @param C: sets the SVM C parameter
     @param useCrossValidation: sets if the SVM model will be trained using cross validation
     @param kFoldValue: sets the number of folds that will be used for cross validation
     @return returns true if the SVM was initialized, false otherwise
     */
    bool init(KernelType kernelType,SVMType svmType,bool useScaling,bool useNullRejection,bool useAutoGamma,
              float gamma,
              unsigned int degree,
              float coef0,
              float nu,
              float C,
              bool useCrossValidation,
              unsigned int kFoldValue
              );
    
    size_t getNumInputs() const override;
    std::vector<size_t> getWhichInputs() const override;
        
#ifndef EMSCRIPTEN
    void getJSONDescription(Json::Value &currentModel) override;
#endif
    
private:
    bool problemSet;
    struct LIBSVM::svm_model *model;
    struct LIBSVM::svm_parameter param;
    struct LIBSVM::svm_problem problem;
    
    int numInputs;
    
    /** Normalization parameters */
    std::vector<double> inRanges;
    std::vector<double> inBases;
    
    bool trained;
};
