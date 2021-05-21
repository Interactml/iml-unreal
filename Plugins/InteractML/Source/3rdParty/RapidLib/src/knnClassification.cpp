//
//  knnClassification.cpp
//  RapidLib
//
//  Created by mzed on 05/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#include <math.h>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include "knnClassification.h"
#ifdef EMSCRIPTEN
#include "emscripten/knnEmbindings.h"
#endif

template<typename T>
knnClassification<T>::knnClassification(const int &num_inputs, const std::vector<int> &which_inputs, const std::vector<trainingExampleTemplate<T> > &_neighbours, const int k)
: numInputs(num_inputs),
whichInputs(which_inputs),
neighbours(_neighbours),
desiredK(k),
currentK(k)
{
    nearestNeighbours = new std::pair<int, T>[currentK];
}

template<typename T>
knnClassification<T>::~knnClassification() {
    delete[] nearestNeighbours;
}

template<typename T>
void knnClassification<T>::reset() {
    //TODO: implement this
}

template<typename T>
int knnClassification<T>::getNumInputs() const {
    return numInputs;
}

template<typename T>
std::vector<int> knnClassification<T>::getWhichInputs() const {
    return whichInputs;
}

template<typename T>
int knnClassification<T>::getK() const {
    return currentK;
}

template<typename T>
inline void knnClassification<T>::updateK() {
    if (currentK != desiredK) {
        currentK = std::min(desiredK, (int) neighbours.size());
    }
}

template<typename T>
void knnClassification<T>::setK(int newK) {
    desiredK = newK;
    updateK();
}

template<typename T>
void knnClassification<T>::addNeighbour(const int &classNum, const std::vector<T> &features) {
    std::vector<T> classVec;
    classVec.push_back(T(classNum));
    trainingExampleTemplate<T>  newNeighbour = {features, classVec};
    neighbours.push_back(newNeighbour);
    updateK();
};

template<typename T>
void knnClassification<T>::train(const std::vector<trainingExampleTemplate<T> > &trainingSet) { //FIXME: Does numInputs need to be reset here? -MZ
    neighbours.clear();
    neighbours = trainingSet;
    updateK();
};

template<typename T>
T knnClassification<T>::run(const std::vector<T> &inputVector) {
    for (int i = 0; i < currentK; ++i) {
        nearestNeighbours[i] = {0, 0.};
    };
    std::pair<int, T> farthestNN {0, 0.};
    
    std::vector<T> pattern;
    for (int h = 0; h < numInputs; h++) {
        pattern.push_back(inputVector[whichInputs[h]]);
    }
    
    //Find k nearest neighbours
    int index = 0;
    for (auto it = neighbours.cbegin(); it != neighbours.cend(); ++it) {
        //find Euclidian distance for this neighbor
        T euclidianDistance = 0;
        for(int j = 0; j < numInputs ; ++j){
            euclidianDistance += pow((pattern[j] - it->input[j]), 2);
        }
        euclidianDistance = sqrt(euclidianDistance);
        if (index < currentK) {
            //save the first k neighbours
            nearestNeighbours[index] = {index, euclidianDistance};
            if (euclidianDistance > farthestNN.second) {
                farthestNN = {index, euclidianDistance};
            }
        } else if (euclidianDistance < farthestNN.second) {
            //replace farthest, if new neighbour is closer
            nearestNeighbours[farthestNN.first] = {index, euclidianDistance};
            int currentFarthest = 0;
            T currentFarthestDistance = 0.;
            for (int n = 0; n < currentK; n++) {
                if (nearestNeighbours[n].second > currentFarthestDistance) {
                    currentFarthest = n;
                    currentFarthestDistance = nearestNeighbours[n].second;
                }
            }
            farthestNN = {currentFarthest, currentFarthestDistance};
        }
        ++index;
    }
    
    //majority vote on nearest neighbours
    std::map<int, int> classVoteMap;
    using classVotePair = std::pair<int, int>;
    for (int i = 0; i < currentK; ++i) {
        int classNum = (int) round(neighbours[nearestNeighbours[i].first].output[0]);
        if ( classVoteMap.find(classNum) == classVoteMap.end() ) {
            classVoteMap.insert(classVotePair(classNum, 1));
        } else {
            ++classVoteMap[classNum];
        }
    }
    T foundClass = 0;
    int mostVotes = 0;
    for (auto p = classVoteMap.cbegin(); p != classVoteMap.cend(); ++p) {
        if (p->second > mostVotes) {
            mostVotes = p->second;
            foundClass = p->first;
        }
    }
    return foundClass;
}

#ifndef EMSCRIPTEN
template<typename T>
void knnClassification<T>::getJSONDescription(Json::Value &jsonModelDescription) {
    jsonModelDescription["modelType"] = "kNN Classificiation";
    jsonModelDescription["numInputs"] = numInputs;
    jsonModelDescription["whichInputs"] = this->vector2json(whichInputs);
    jsonModelDescription["k"] = desiredK;
    Json::Value examples;
    for (auto it = neighbours.cbegin(); it != neighbours.cend(); ++it) {
        Json::Value oneExample;
        oneExample["class"] = it->output[0];
        oneExample["features"] = this->vector2json(it->input);
        examples.append(oneExample);
    }
    jsonModelDescription["examples"] = examples;
}
#endif

//explicit instantiation
template class knnClassification<double>;
template class knnClassification<float>;
