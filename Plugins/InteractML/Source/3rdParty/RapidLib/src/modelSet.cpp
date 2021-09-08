//
//
//  modelSet.cpp
//  RapidLib
//
//  Created by mzed on 26/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//


#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include "modelSet.h"

#ifndef EMSCRIPTEN
#include "../dependencies/json/json.h"
#else
#include "emscripten/modelSetEmbindings.h"
#endif

/** No arguments, don't create any models yet */
template<typename T> 
modelSet<T>::modelSet() :
    numInputs(-1),
    numOutputs(-1),
    isTraining(false),
    isTrained(false)
{
};

template<typename T>
modelSet<T>::~modelSet() 
{
    for (auto& model : myModelSet) 
    {
        delete model;
    }
};

template<typename T>
bool modelSet<T>::train(const std::vector<trainingExampleTemplate<T> > &training_set) 
{
    bool success = false;
    if (isTraining)
    {
        throw std::runtime_error("model is already training");
    }
    else
    {
        for (trainingExampleTemplate<T> example : training_set)
        {
            if (example.input.size() != numInputs)
            {
                throw std::length_error("unequal feature vectors in input.");
                return false;
            }
            if (example.output.size() != numOutputs)
            {
                throw std::length_error("unequal output vectors.");
                return false;
            }
        }

        // Multithreaded training
        std::vector<std::thread> trainingThreads;
        for (std::size_t i = 0; i < myModelSet.size(); ++i)
        {
            trainingThreads.push_back(std::thread(&modelSet<T>::threadTrain, this, i, training_set));
        }

        for (std::size_t i = 0; i < myModelSet.size(); ++i)
        {
            trainingThreads.at(i).join();
        }
        isTraining = false;
        success = isTrained = true;
    }
    return success;
}

template<typename T>
void modelSet<T>::threadTrain(std::size_t i, const std::vector<trainingExampleTemplate<T> > &training_set) 
{
    myModelSet[i]->train(training_set, i);
}

template<typename T>
bool modelSet<T>::reset() 
{
    for (auto& model : myModelSet) 
    {
        delete model;
    }
    myModelSet.clear();
    numInputs = -1;
    numOutputs = -1;
    isTraining = false;
    return true;
}

template<typename T>
std::vector<T> modelSet<T>::run(const std::vector<T> &inputVector) 
{
    std::vector<T> returnVector;

    if (isTraining)
    {
        throw std::runtime_error("can't run a model during training");
        returnVector.push_back(0);
    }
    else if (inputVector.size() != numInputs) 
    {
        std::string badSize = std::to_string(inputVector.size());
        throw std::length_error("bad input size: " + badSize);
        returnVector.push_back(0);
    } 
    else 
    {
        for (auto model : myModelSet)
        {
            returnVector.push_back(model->run(inputVector));
        }
    }
    return returnVector;
}



#ifndef EMSCRIPTEN
//In emscripten, we do the JSON parsing with native JavaScript
template<typename T>
std::vector<T> json2vector(Json::Value json) 
{
    std::vector<T> returnVec;
    for (auto jsonValue : json) 
    {
        returnVec.push_back((T)jsonValue.asDouble());
    }
    return returnVec;
}

template<typename T>
Json::Value modelSet<T>::parse2json() 
{
    Json::Value root;
    Json::Value metadata;
    Json::Value modelSet;
    
    metadata["creator"] = "Rapid API C++";
    metadata["version"] = "v0.1.1"; //TODO: This should be a macro someplace
    metadata["numInputs"] = numInputs;
    Json::Value inputNamesJSON;

    for (size_t i = 0; i < inputNames.size(); ++i) 
    {
        inputNamesJSON.append(inputNames[i]);
    }
    metadata["inputNames"] = inputNamesJSON;
    metadata["numOutputs"] = numOutputs;
    root["metadata"] = metadata;
    for (auto model : myModelSet) 
    {
        Json::Value currentModel;
        currentModel["inputNames"] = inputNamesJSON; //TODO: implment this feature
        model->getJSONDescription(currentModel);
        modelSet.append(currentModel);
    }
    root["modelSet"] = modelSet;
    return root;
}

template<typename T>
std::string modelSet<T>::getJSON() 
{
    Json::Value root = parse2json();
    return root.toStyledString();
}

template<typename T>
void modelSet<T>::writeJSON(const std::string &filepath) 
{
    Json::Value root = parse2json();
    std::ofstream jsonOut;
    jsonOut.open (filepath);
    Json::StyledStreamWriter writer;
    writer.write(jsonOut, root);
    jsonOut.close();
    
}

template<typename T>
bool modelSet<T>::putJSON(const std::string &jsonMessage) 
{
    Json::Value parsedFromString;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonMessage, parsedFromString);
    if (parsingSuccessful) json2modelSet(parsedFromString);
    return parsingSuccessful;
}

template<typename T>
void modelSet<T>::json2modelSet(const Json::Value &root) 
{
    numInputs = root["metadata"]["numInputs"].asInt();

    for (unsigned int i = 0; i < root["metadata"]["inputNames"].size(); ++i) 
    {
        inputNames.push_back(root["metadata"]["inputNames"][i].asString());
    }
    numOutputs = root["metadata"]["numOutputs"].asInt();
    
    for (const Json::Value& model : root["modelSet"]) 
    {
        int modelNumInputs = model["numInputs"].asInt();
        std::vector<size_t> whichInputs;
        std::vector<std::string> modelInputNames;

        for (unsigned int i = 0; i < model["inputNames"].size(); ++i) 
        {
            modelInputNames.push_back(model["inputNames"][i].asString());
        }

        for (size_t i = 0; i < inputNames.size(); ++i) 
        {
            if (std::find(modelInputNames.begin(), modelInputNames.end(), inputNames[i]) != modelInputNames.end())
            {
                whichInputs.push_back(i);
            }
        }

        if (model["modelType"].asString() == "Neural Network") 
        {
            int numHiddenLayers = model["numHiddenLayers"].asInt();
            int numHiddenNodes = model["numHiddenNodes"].asInt();
            std::vector<T> weights;
            std::vector<T> wHiddenOutput;
            int nodeIndex = 0;

            for (const Json::Value& node : model["nodes"]) 
            {
                if (node["name"].asString() == "Linear Node 0") 
                {
                    for (int i = 1; i <= numHiddenNodes; ++i)
                    {
                        std::string whichNode = "Node " + std::to_string(i + (numHiddenNodes * (numHiddenLayers - 1)));
                        wHiddenOutput.push_back((T)node[whichNode].asDouble());
                    }
                    wHiddenOutput.push_back(node["Threshold"].asDouble());
                } 
                else 
                { //FIXME: this will break if nodes are out of order
                    int currentLayer = (int) floor((nodeIndex - 1.0)/ (double)numHiddenNodes);
                    if (currentLayer < 1) //Nodes connected to input
                    { 
                        for (int i = 0; i < numInputs; ++i) 
                        {
                            std::string whichNode = "Attrib " + model["inputNames"][i].asString();
                            weights.push_back(node[whichNode].asDouble());
                        }
                    } 
                    else //Hidden Layers
                    { 
                        for (int i = 0; i < numHiddenNodes; ++i)
                        {
                            std::string whichNode = "Node " + std::to_string(i + (numHiddenNodes * (currentLayer - 1)));
                            weights.push_back(node[whichNode].asDouble());
                        }                    }
                    weights.push_back(node["Threshold"].asDouble());
                }
                nodeIndex++;
            }
            std::vector<T> inBases = json2vector<T>(model["inBases"]);
            std::vector<T> inRanges = json2vector<T>(model["inRanges"]);
            T outRange = (T)model["outRange"].asDouble();
            T outBase = (T)model["outBase"].asDouble();
            
            //TODO: many of these arguments could be size_t
            myModelSet.push_back(new neuralNetwork<T>(modelNumInputs, whichInputs, numHiddenLayers, numHiddenNodes, weights, wHiddenOutput, inRanges, inBases, outRange, outBase));
        } 
        else if (model["modelType"].asString() == "kNN Classificiation") 
        {
            std::vector<trainingExampleTemplate<T> > trainingSet;
            const Json::Value examples = model["examples"];

            for (unsigned int i = 0; i < examples.size(); ++i) 
            {
                trainingExampleTemplate<T> tempExample;
                tempExample.input = json2vector<T>(examples[i]["features"]);
                tempExample.output.push_back((T)examples[i]["class"].asDouble());
                trainingSet.push_back(tempExample);
            }
            int k = model["k"].asInt();

            myModelSet.push_back(new knnClassification<T>(modelNumInputs, whichInputs, trainingSet, k));
        }
    }
}

template<typename T>
bool modelSet<T>::readJSON(const std::string &filepath) 
{
    Json::Value root;
    std::ifstream file(filepath);
    file >> root;
    json2modelSet(root);
    return true; //TODO: check something first
}
#endif

//explicit instantiation
template class modelSet<double>;
template class modelSet<float>;

