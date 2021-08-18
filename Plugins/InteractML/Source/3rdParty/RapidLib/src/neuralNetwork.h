/**
 * @file neuralNetwork.h
 *  RapidLib
 *
 * @date 05 Sep 2016
 * @copyright Copyright © 2016 Goldsmiths. All rights reserved.
 */

#pragma once

#include <vector>
#include "baseModel.h"

#ifndef EMSCRIPTEN
#include "../dependencies/json/json.h"
#endif

 /*! Class for implementing a Neural Network.
  *
  * This class includes both running and training, and constructors for reading trained models from JSON.
  */
template<typename T>
class neuralNetwork final : public baseModel<T>
{
public:
    /** This is the constructor for building a trained model from JSON. */
    neuralNetwork(const size_t& num_inputs,
        const std::vector<size_t>& which_inputs,
        const size_t& num_hidden_layers,
        const size_t& num_hidden_nodes,
        const std::vector<T>& weights,
        const std::vector<T>& wHiddenOutput,
        const std::vector<T>& inRanges,
        const std::vector<T>& inBases,
        const T& outRange,
        const T& outBase);

    /** This constructor creates a neural network that needs to be trained.
     *
     * @param num_inputs is the number of inputs the network will process
     * @param which_inputs is an vector of which values in the input vector are being fed to the network. ex: {0,2,4}
     * @param num_hidden_layer is the number of hidden layers in the network. Must be at least 1.
     * @param num_hidden_nodes is the number of hidden nodes in each hidden layer. Often, this is the same as num_inputs
     *
     * @return A neuralNetwork instance with randomized weights and no normalization values. These will be set or adjusted during training.
     */
    neuralNetwork(const size_t& num_inputs,
        const std::vector<size_t>& which_inputs,
        const size_t& num_hidden_layer,
        const size_t& num_hidden_nodes);

    /** destructor */
    ~neuralNetwork();

    /** Generate an output value from a single input vector.
     * @param vector A standard vector of type T that feed-forward regression will run on.
     * @return T A single value, which is the result of the feed-forward operation
     */
    T run(const std::vector<T>& inputVector) override;

    void reset() override;

    size_t getNumInputs() const override;
    std::vector<size_t> getWhichInputs() const override;

    size_t getNumHiddenLayers() const;
    void setNumHiddenLayers(int num_hidden_layers);

    size_t getNumHiddenNodes() const;
    void setNumHiddenNodes(int num_hidden_nodes);

    size_t getEpochs() const;
    void setEpochs(const size_t& epochs);

    std::vector<T> getWeights() const;
    std::vector<T> getWHiddenOutput() const;

    std::vector<T> getInRanges() const;
    std::vector<T> getInBases() const;
    T getOutRange() const;
    T getOutBase() const;

#ifndef EMSCRIPTEN
    void getJSONDescription(Json::Value& currentModel) override;
#endif


private:
    /** Parameters that describe the topography of the model */
    int numInputs;
    std::vector<size_t> whichInputs;
    int numHiddenLayers;
    int numHiddenNodes;

    /** Neurons: state is updated on each process(). */
    std::vector<T> inputNeurons;
    std::vector<std::vector<T> > hiddenNeurons;
    T outputNeuron;

    /** Weights between layers and nodes are kept here. */
    std::vector<std::vector<std::vector<T> > > weights;
    std::vector<T> wHiddenOutput;

    /** Normalization parameters */
    std::vector<T> inRanges;
    std::vector<T> inBases;
    T outRange;
    T outBase;

    /** Sigmoid function for activating hidden nodes. */
    inline T activationFunction(T);

    ////////////////////////////////////////////////////////////////////////////
    /// These pertain to the training, and aren't need to run a trained model //

public:
    /** Train a model using backpropagation.
     *
     * @param The training set is a vector of training examples that contain both a vector of input values and a value specifying desired output.
     *
     */
    void train(const std::vector<trainingExampleTemplate<T> >& trainingSet) override;

    /** Train a model using backpropagation. This function is used when the model is part of a modelSet.
     *
     * @param This triaining function takes examples that contain a vector of input values and a vector of output values.
     * @param The second argument specifies which output this model is using.
     *
     */
    void train(const std::vector<trainingExampleTemplate<T> >& trainingSet, const std::size_t whichOutput) override;

    /** Returns current training epoch. */
    size_t getCurrentEpoch() const;

private:
    /** Parameters that influence learning */
    T learningRate = 0.3;
    T momentum = 0.2;
    size_t numEpochs = 500;
    size_t currentEpoch = 0;

    /** These deltas are applied to the weights in the network */
    std::vector<std::vector< std::vector<T> > > deltaWeights;
    std::vector<T> deltaHiddenOutput;

    /** Parameters and functions for calculating amount of change for each weight */
    T outputErrorGradient;
    inline T getHiddenErrorGradient(int layer, int neuron);

    void initTrainer();

    /** Propagate output error back through the network.
     * @param The desired output of the network is fed into the function, and compared with the actual output
     */
    void backpropagate(const T& desiredOutput);

    /** Apply corrections to network weights, based on output error */
    void updateWeights();
};
