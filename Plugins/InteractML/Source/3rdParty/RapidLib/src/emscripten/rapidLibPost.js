==== BASE ====
//
//  rapidLibPost.js
//  RapidLib
//
//  Created by mzed on 05/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

/* globals Module */

"use strict";

console.log("RapidLib 19.12.2017 10:59");

/**
 * Utility function to convert js objects into C++ trainingSets
 * @param {Object} trainingSet - JS Object representing a training set
 * @property {function} Module.TrainingSet - constructor for emscripten version of this struct
 * @property {function} Module.VectorDouble - constructor for the emscripten version of std::vector<double>
 * @returns {Module.TrainingSet}
 */
Module.prepTrainingSet = function (trainingSet) {
    let rmTrainingSet = new Module.TrainingSet();
    for (let i = 0; i < trainingSet.length; ++i) {
        let tempInput = new Module.VectorDouble();
        let tempOutput = new Module.VectorDouble();
        for (let j = 0; j < trainingSet[i].input.length; ++j) {
            tempInput.push_back(parseFloat(trainingSet[i].input[j]));
        }
        for (let j = 0; j < trainingSet[i].output.length; ++j) {
            tempOutput.push_back(parseFloat(trainingSet[i].output[j]));
        }
        let tempObj = {'input': tempInput, 'output': tempOutput};
        rmTrainingSet.push_back(tempObj);
    }
    return rmTrainingSet;
};

/**
 * Utility function to convert js objects into C++ trainingSeriesSets
 * @param {Object} trainingSeriesSet - JS Object representing a training series
 * @property {function} Module.TrainingSeries - constructor for emscripten version of this struct
 * @property {function} Module.VectorVectorDouble - constructor for the emscripten version of std::vector<std::vector<double>>
 * @returns {Module.TrainingSeriesSet}
 */
Module.prepTrainingSeriesSet = function (trainingSeriesSet) {
    let rmTrainingSeriesSet = new Module.TrainingSeriesSet();
    for (let i = 0; i < trainingSeriesSet.length; ++i) {
        let input = new Module.VectorVectorDouble();
        for (let j = 0; j < trainingSeriesSet[i].input.length; ++j) {
            let tempVector = new Module.VectorDouble();
            for (let k = 0; k < trainingSeriesSet[i].input[j].length; ++k) {
                tempVector.push_back(parseFloat(trainingSeriesSet[i].input[j][k]));
            }
            input.push_back(tempVector);
        }
        let tempObj = {'input': input, 'label': trainingSeriesSet[i].label};
        rmTrainingSeriesSet.push_back(tempObj);
    }
    return rmTrainingSeriesSet;
};

/**
 * Utility function to add an empty output to a "training set" if it is undefined
 * @param jsInput
 * @returns {*}
 */

Module.checkOutput = function (jsInput) {
    for (let i = 0; i < jsInput.length; ++i) {
        if (typeof jsInput[i].output === "undefined") {
            jsInput[i].output = [];
        }
    }
    return jsInput;
};
////////////////////////////////////////////////   Regression

/**
 * Creates a set of regression objects using the constructor from emscripten
 * @constructor
 * @property {function} Module.RegressionCpp - constructor from emscripten
 */
Module.Regression = function () {
    this.modelSet = new Module.RegressionCpp(); //TODO implement optional arguments
};

Module.Regression.prototype = {
    /**
     * Trains the models using the input. Starts training from a randomized state.
     * @param {Object} trainingSet - An array of training examples
     * @returns {Boolean} true indicates successful training
     */
    train: async function (trainingSet) { // FIXME: AFAIK Babel doesn't like ES7
        this.modelSet.reset();
        //change to vectorDoubles and send in
        try {
            await this.modelSet.train(Module.prepTrainingSet(trainingSet)); // FIXME: AFAIK Babel doesn't like ES7
            console.log("AHAHAHAHAHAH!!!");
        } catch (err) {
            console.log(err);
            return false;
        }
        return true;
    },
    /**
     * Returns the number of hidden layers in a MLP.
     * @returns {Number} k values
     */
    getNumHiddenLayers: function () {
        let outputVector = this.modelSet.getNumHiddenLayers();
        //change back to javascript array
        let output = [];
        for (let i = 0; i < outputVector.size(); ++i) {
            output.push(outputVector.get(i));
        }
        return output[0];
    },
    /**
     * Sets the number of hidden layers for an MLP.
     * @param {Number} numHiddenLayers
     */
    setNumHiddenLayers: function (numHiddenLayers) {
        this.modelSet.setNumHiddenLayers(numHiddenLayers);
    },
    /**
     * Returns the number of hidden nodes in a MLP.
     * @returns {Number} hidden node values
     */
    getNumHiddenNodes: function () {
        let outputVector = this.modelSet.getNumHiddenNodes();
        //change back to javascript array
        let output = [];
        for (let i = 0; i < outputVector.size(); ++i) {
            output.push(outputVector.get(i));
        }
        return output[0];
    },
    /**
     * Sets the number of hidden nodes for an MLP.
     * @param {Number} numHiddenNodes
     */
    setNumHiddenNodes: function (numHiddenNodes) {
        this.modelSet.setNumHiddenNodes(numHiddenNodes);
    },
    /**
     * Sets the number of epochs for MLP training.
     * @param {Number} numEpochs
     */
    setNumEpochs: function (numEpochs) {
        this.modelSet.setNumEpochs(numEpochs);
    },
    /**
     * Returns the model set to its initial configuration.
     * @returns {Boolean} true indicates successful initialization
     */
    reset: function () {
        return this.modelSet.reset();
    },
    /**
     * Runs feed-forward regression on input
     * @param {Array} input - An array of features to be processed. Non-arrays are converted.
     * @returns {Array} output - One number for each model in the set
     */
    run: function (input) {
        //I'll assume that the args should have been an array
        if (arguments.length > 1) {
            input = Array.from(arguments);
        }
        //change input to vectors of doubles
        let inputVector = new Module.VectorDouble();
        for (let i = 0; i < input.length; ++i) {
            inputVector.push_back(input[i]);
        }
        //get the output
        let outputVector = new Module.VectorDouble();
        try {
            outputVector = this.modelSet.run(inputVector);
        } catch (err) {
            console.warn(err);
            return [0];
        }
        //change back to javascript array
        let output = [];
        for (let i = 0; i < outputVector.size(); ++i) {
            output.push(outputVector.get(i));
        }
        return output;
    },
    /**
     * Deprecated! Use run() instead
     * @param input
     * @returns {Array}
     */
    process: function (input) {
        //return this.run(input); //Why doesn't this work? MZ
        //I'll assume that the args should have been an array
        if (arguments.length > 1) {
            input = Array.from(arguments);
        }
        //change input to vectors of doubles
        let inputVector = new Module.VectorDouble();
        for (let i = 0; i < input.length; ++i) {
            inputVector.push_back(input[i]);
        }
        //get the output
        let outputVector = new Module.VectorDouble();
        outputVector = this.modelSet.run(inputVector);
        //change back to javascript array
        let output = [];
        for (let i = 0; i < outputVector.size(); ++i) {
            output.push(outputVector.get(i));
        }
        return output;
    }
};

/////////////////////////////////////////////////  Classification

/**
 * Creates a set of classification objects using the constructor from emscripten
 * @constructor
 * @property {function} Module.ClassificationCpp - constructor from emscripten
 * @param {string} [type] - which classification algorithm to use
 */

Module.Classification = function (type) {
    if (type) {
        this.modelSet = new Module.ClassificationCpp(type);
    } else {
        this.modelSet = new Module.ClassificationCpp();
    }
};

Module.Classification.prototype = {
    /**
     * Trains the models using the input. Clears previous training set.
     * @param {Object} trainingSet - An array of training examples.
     * @returns {Boolean} true indicates successful training
     */
    train: function (trainingSet) {
        this.modelSet.reset();
        try {
            this.modelSet.train(Module.prepTrainingSet(trainingSet));
        } catch (err) {
            console.log(err);
            return false;
        }
        return true;
    },
    /**
     * Returns a vector of current k values for each model.
     * @returns {Array} k values
     */
    getK: function () {
        let outputVector = this.modelSet.getK();
        let output = [];
        for (let i = 0; i < outputVector.size(); ++i) {
            output.push(outputVector.get(i));
        }
        return output;
    },
    /**
     * Sets the k values for a particular model model.
     * @param {Number} whichModel - which model
     * @param {Number} newK - set K to this value
     */
    setK: function (whichModel, newK) {
        this.modelSet.setK(whichModel, newK);
    },
    /**
     * Returns the model set to its initial configuration.
     * @returns {Boolean} true indicates successful initialization
     */
    reset: function () {
        return this.modelSet.reset();
    },
    /**
     * Does classifications on an input vector.
     * @param {Array} input - An array of features to be processed. Non-arrays are converted.
     * @returns {Array} output - One number for each model in the set
     */
    run: function (input) {
        //I'll assume that the args should have been an array
        if (arguments.length > 1) {
            input = Array.from(arguments);
        }
        //change input to vectors of doubles
        let inputVector = new Module.VectorDouble();
        for (let i = 0; i < input.length; ++i) {
            inputVector.push_back(input[i]);
        }
        //get the output
        let outputVector = new Module.VectorDouble();
        try {
            outputVector = this.modelSet.run(inputVector);
        } catch (err) {
            console.log(err);
            return [0];
        }
        //change back to javascript array
        let output = [];
        for (let i = 0; i < outputVector.size(); ++i) {
            output.push(outputVector.get(i));
        }
        return output;
    },
    /**
     * Deprecated! USe run() instead
     * @param input
     */
    process: function (input) {
        //return this.run(input); //why doesn't this work?
        //I'll assume that the args should have been an array
        if (arguments.length > 1) {
            input = Array.from(arguments);
        }
        //change input to vectors of doubles
        let inputVector = new Module.VectorDouble();
        for (let i = 0; i < input.length; ++i) {
            inputVector.push_back(input[i]);
        }
        //get the output
        let outputVector = this.modelSet.run(inputVector);
        //change back to javascript array
        let output = [];
        for (let i = 0; i < outputVector.size(); ++i) {
            output.push(outputVector.get(i));
        }
        return output;
    }
};

//////////////////////////////////////////////////  ModelSet

/**
 * Creates a set of machine learning objects using constructors from emscripten. Could be any mix of regression and classification.
 * This is only useful when importing JSON from Wekinator.
 * @constructor
 */
Module.ModelSet = function () {
    this.myModelSet = [];
    this.modelSet = new Module.ModelSetCpp();
};

/**
 * Creates a model set populated with models described in a JSON document.
 * This only works in documents that are part of a CodeCircle document.
 * @param {string} url - JSON loaded from a model set description document.
 * @returns {Boolean} true indicates successful training
 */
Module.ModelSet.prototype = {
    loadJSON: function (url) {
        let that = this;
        console.log('url ', url);
        let request = new XMLHttpRequest();
        request.open("GET", url, true);
        request.responseType = "json";
        request.onload = function () {
            let modelSet = this.response;
            console.log("loaded: ", modelSet);
            let allInputs = modelSet.metadata.inputNames;
            modelSet.modelSet.forEach(function (value) {
                let numInputs = value.numInputs;
                let whichInputs = new Module.VectorInt();
                switch (value.modelType) {
                    case 'kNN classification':
                        let neighbours = new Module.TrainingSet();
                        let k = value.k;
                        for (let i = 0; i < allInputs.length; ++i) {
                            if (value.inputNames.includes(allInputs[i])) {
                                whichInputs.push_back(i);
                            }
                        }
                        let myKnn = new Module.KnnClassification(numInputs, whichInputs, neighbours, k);
                        value.examples.forEach(function (value) {
                            let features = new Module.VectorDouble();
                            for (let i = 0; i < numInputs; ++i) {
                                features.push_back(parseFloat(value.features[i]));
                            }
                            myKnn.addNeighbour(parseInt(value.class), features);
                        });
                        that.addkNNModel(myKnn);
                        break;
                    case 'Neural Network':
                        let numLayers = value.numHiddenLayers;
                        let numNodes = value.numHiddenNodes;
                        let weights = new Module.VectorDouble();
                        let wHiddenOutput = new Module.VectorDouble();
                        let inRanges = new Module.VectorDouble();
                        let inBases = new Module.VectorDouble();

                        let localWhichInputs = [];
                        for (let i = 0; i < allInputs.length; ++i) {
                            if (value.inputNames.includes(allInputs[i])) {
                                whichInputs.push_back(i);
                                localWhichInputs.push(i);
                            }
                        }

                        let currentLayer = 0;
                        value.nodes.forEach(function (value, i) {
                            if (value.name === 'Linear Node 0') { //Output Node
                                for (let j = 1; j <= numNodes; ++j) {
                                    let whichNode = 'Node ' + (j + (numNodes * (numLayers - 1)));
                                    wHiddenOutput.push_back(parseFloat(value[whichNode]));
                                }
                                wHiddenOutput.push_back(parseFloat(value.Threshold));
                            } else {
                                currentLayer = Math.floor((i - 1) / numNodes); //FIXME: This will break if node is out or order.
                                if (currentLayer < 1) { //Nodes connected to input
                                    for (let j = 0; j < numInputs; ++j) {
                                        weights.push_back(parseFloat(value['Attrib ' + allInputs[localWhichInputs[j]]]));
                                    }
                                } else { //Hidden Layers
                                    for (let j = 1; j <= numNodes; ++j) {
                                        weights.push_back(parseFloat(value['Node ' + (j + (numNodes * (currentLayer - 1)))]));
                                    }
                                }
                                weights.push_back(parseFloat(value.Threshold));
                            }
                        });

                        for (let i = 0; i < numInputs; ++i) {
                            inRanges.push_back(value.inRanges[i]);
                            inBases.push_back(value.Bases[i]);
                        }

                        let outRange = value.outRange;
                        let outBase = value.outBase;

                        let myNN = new Module.NeuralNetwork(numInputs, whichInputs, numLayers, numNodes, weights, wHiddenOutput, inRanges, inBases, outRange, outBase);
                        that.addNNModel(myNN);
                        break;
                    default:
                        console.warn('unknown model type ', value.modelType);
                        break;
                }
            });
        };
        request.send(null);
        return true; //TODO: make sure this is true;
    },
    /**
     * Add a NN model to a modelSet. //TODO: this doesn't need it's own function
     * @param model
     */
    addNNModel: function (model) {
        console.log('Adding NN model');
        this.myModelSet.push(model);
    },
    /**
     * Add a kNN model to a modelSet. //TODO: this doesn't need it's own function
     * @param model
     */
    addkNNModel: function (model) {
        console.log('Adding kNN model');
        this.myModelSet.push(model);
    },
    /**
     * Applies regression and classification algorithms to an input vector.
     * @param {Array} input - An array of features to be processed.
     * @returns {Array} output - One number for each model in the set
     */
    run: function (input) {
        let modelSetInput = new Module.VectorDouble();
        for (let i = 0; i < input.length; ++i) {
            modelSetInput.push_back(input[i]);
        }
        let output = [];
        for (let i = 0; i < this.myModelSet.length; ++i) {
            output.push(this.myModelSet[i].run(modelSetInput));
        }
        return output;
    },
    /**
     * Deprecated! Use run() instead.
     * @param {Array} input - An array of features to be processed
     * @returns {Array} output - One number for each model in the set
     */
    process: function (input) {
        return this.run(input);
    }
};


////////////////////////////////////////////////

/**
 * Creates a series classification object using the constructor from emscripten
 * @constructor
 * @property {function} Module.SeriesClassificationCpp - constructor from emscripten
 */
Module.SeriesClassification = function () {
    this.seriesClassification = new Module.SeriesClassificationCpp(); //TODO implement optional arguments
};

Module.SeriesClassification.prototype = {
    /**
     * Resets the model, and adds a set of series to be evaluated
     * @param {Object} newSeriesSet - an array of objects, each with input: <array of arrays> and label: <string>
     * @return {Boolean} True indicates successful training.
     */
    train: function (newSeriesSet) {
        this.reset();
        this.seriesClassification.train(Module.prepTrainingSeriesSet(newSeriesSet));
        return true;
    },
    /**
     * Returns the model set to its initial configuration.
     * @returns {Boolean} true indicates successful initialization
     */
    reset: function () {
        return this.seriesClassification.reset();
    },
    /**
     * Evaluates an input series and returns the index of the closet example
     * @param {Object} inputSeries - an array of arrays
     * @param {String} label to match against
     * @returns {Number} The index of the closest matching series
     */
    run: function (inputSeries, label) {
        let vecInputSeries = new Module.VectorVectorDouble();
        for (let i = 0; i < inputSeries.length; ++i) {
            let tempVector = new Module.VectorDouble();
            for (let j = 0; j < inputSeries[i].length; ++j) {
                tempVector.push_back(inputSeries[i][j]);
            }
            vecInputSeries.push_back(tempVector);
        }
        if (arguments.length > 1) {
            return this.seriesClassification.runLabel(vecInputSeries, label);
        } else {
            return this.seriesClassification.run(vecInputSeries);
        }
    },
    /**
     * Deprecated! Use run()
     * @param inputSeries
     * @returns {Number}
     */
    process: function (inputSeries) {
        return this.run(inputSeries);
    },
    /**
     * Returns an array of costs to match the input series to each example series. A lower cost is a closer match
     * @returns {Array}
     */
    getCosts: function () {
        let returnArray = [];
        let VecDouble = this.seriesClassification.getCosts();
        for (let i = 0; i < VecDouble.size(); ++i) {
            returnArray[i] = VecDouble.get(i);
        }
        return returnArray;
    }
};

/////////////////////////////////////////////////

/**
 * Creates a circular buffer that can return various statistics
 * @constructor
 * @param {number} [windowSize=3] - specify the size of the buffer
 * @property {function} Module.rapidStreamCpp - constructor from emscripten
 */

Module.StreamBuffer = function (windowSize) {
    if (windowSize) {
        this.rapidStream = new Module.RapidStreamCpp(parseFloat(windowSize));
    } else {
        this.rapidStream = new Module.RapidStreamCpp();
    }
};

Module.StreamBuffer.prototype = {
    /**
     * Add a value to a circular buffer whose size is defined at creation.
     * @param {number} input - value to be pushed into circular buffer.
     */
    push: function (input) {
        this.rapidStream.pushToWindow(parseFloat(input));
    },
    /**
     * Resets all the values in the buffer to zero.
     */
    reset: function () {
        this.rapidStream.clear();
    },
    /**
     * Calculate the first-order difference (aka velocity) between the last two inputs.
     * @return {number} difference between last two inputs.
     */
    velocity: function () {
        return this.rapidStream.velocity();
    },
    /**
     * Calculate the second-order difference (aka acceleration) over the last three inputs.
     * @return {number} acceleration over the last three inputs.
     */
    acceleration: function () {
        return this.rapidStream.acceleration();
    },
    /**
     * Find the minimum value in the buffer.
     * @return {number} minimum.
     */
    minimum: function () {
        return this.rapidStream.minimum();
    },
    /**
     * Find the maximum value in the buffer.
     * @return {number} maximum.
     */
    maximum: function () {
        return this.rapidStream.maximum();
    },
    /**
     * Count the number of zero crossings in the buffer.
     * @return {number} number of zero crossings.
     */
    numZeroCrossings: function () {
        return this.rapidStream.numZeroCrossings();
    },
    /**
     * Calculate the sum of all values in the buffer.
     * @return {number} sum.
     */
    sum: function () {
        return this.rapidStream.sum();
    },
    /**
     * Calculate the mean of all values in the buffer.
     * @return {number} mean.
     */
    mean: function () {
        return this.rapidStream.mean();
    },
    /**
     * Calculate the standard deviation of all values in the buffer.
     * @return {number} standard deviation.
     */
    standardDeviation: function () {
        return this.rapidStream.standardDeviation();
    },
    /**
     * Calculate the root mean square of the values in the buffer
     * @return {number} rms
     */
    rms: function () {
        return this.rapidStream.rms();
    },
    /**
     * Non-linear Bayesian filtering for EMG
     * @return {number} envelope
     */
    bayesFilter: function (input) {
        return this.rapidStream.bayesFilter(parseFloat(input));
    },
    bayesSetDiffusion: function (input) {
        this.rapidStream.bayesSetDiffusion(parseFloat(input));
    },
    bayesSetJumpRate: function (input) {
        this.rapidStream.bayesSetJumpRate(parseFloat(input));
    },
    bayesSetMVC: function (input) {
        this.rapidStream.bayesSetMVC(parseFloat(input));
    },
    /**
     * Calculate the minimum first-order difference over consecutive inputs in the buffer.
     * @return {number} minimum velocity.
     */
    minVelocity: function () {
        return this.rapidStream.minVelocity();
    },
    /**
     * Calculate the maximum first-order difference over consecutive inputs in the buffer.
     * @return {number} maximum velocity.
     */
    maxVelocity: function () {
        return this.rapidStream.maxVelocity();
    },
    /**
     * Calculate the minimum second-order difference over consecutive inputs in the buffer.
     * @return {number} minimum acceleration.
     */
    minAcceleration: function () {
        return this.rapidStream.minAcceleration();
    },
    /**
     * Calculate the maximum second-order difference over consecutive inputs in the buffer.
     * @return {number} maximum acceleration.
     */
    maxAcceleration: function () {
        return this.rapidStream.maxAcceleration();
    }
};
