/**
 * @file dtw.cpp
 * RapidLib
 *
 * @author Michael Zbyszynski
 * @date 07 Jun 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#include <vector>
#include <cmath>
#include <cassert>
#include <limits>
#include "dtw.h"

template<typename T>
dtw<T>::dtw() {};

template<typename T>
dtw<T>::~dtw() {};

template<typename T>
inline T dtw<T>::distanceFunction(const std::vector<T> &x, const std::vector<T> &y)
{
    assert(x.size() == y.size());
    double euclidianDistance = 0;
    
    for(std::size_t j = 0; j < x.size() ; ++j)
    {
        euclidianDistance = euclidianDistance + pow((x[j] - y[j]), 2);
    }
    
    euclidianDistance = sqrt(euclidianDistance);
    return (T)euclidianDistance;
};

/* Just returns the cost, doesn't calculate the path */
template<typename T>
T dtw<T>::getCost(const std::vector<std::vector<T> > &seriesX, const std::vector<std::vector<T> > &seriesY)
{
    if (seriesX.size() < seriesY.size())
    {
        return getCost(seriesY, seriesX);
    }
    
    costMatrix.clear();
    for (std::size_t i = 0; i < seriesX.size(); ++i)
    {
        std::vector<T> tempVector;
        for (std::size_t j = 0; j < seriesY.size(); ++j)
        {
            tempVector.push_back(0);
        }
        costMatrix.push_back(tempVector);
    }
    std::size_t maxX = seriesX.size() - 1;
    std::size_t maxY = seriesY.size() - 1;
    
    //Calculate values for the first column
    costMatrix[0][0] = distanceFunction(seriesX[0], seriesY[0]);
    for (int j = 1; j <= maxY; ++j) {
        costMatrix[0][j] = costMatrix[0][j - 1] + distanceFunction(seriesX[0], seriesY[j]);
    }
    
    for (std::size_t i = 1; i <= maxX; ++i)
    {
        //Bottom row of current column
        costMatrix[i][0] = costMatrix[i - 1][0] + distanceFunction(seriesX[i], seriesY[0]);
        
        for (std::size_t j = 1; j <= maxY; ++j)
        {
            T minGlobalCost = fmin(costMatrix[i-1][j-1], costMatrix[i][j-1]);
            costMatrix[i][j] = minGlobalCost + distanceFunction(seriesX[i], seriesY[j]);
        }
    }
    return costMatrix[maxX][maxY];
};

template<typename T>
warpPath dtw<T>::calculatePath(std::size_t seriesXsize, std::size_t seriesYsize) const
{
    warpPath warpPath;
    std::size_t i = seriesXsize - 1;
    std::size_t j = seriesYsize - 1;
    warpPath.add(i, j);
    
    while ((i > 0) || (j > 0))
    {
        T diagonalCost = ((i > 0) && (j > 0)) ? costMatrix[i - 1][j - 1] : std::numeric_limits<T>::infinity();
        T leftCost = (i > 0) ? costMatrix[i - 1][j] : std::numeric_limits<T>::infinity();
        T downCost = (j > 0) ? costMatrix[i][j - 1] : std::numeric_limits<T>::infinity();

        if ((diagonalCost <= leftCost) && (diagonalCost <= downCost)) 
        {
            if (i > 0) --i;
            if (j > 0) --j;
        } 
        else if ((leftCost < diagonalCost) && (leftCost < downCost))
        {
            --i;
        } 
        else if ((downCost < diagonalCost) && (downCost < leftCost)) 
        {
            --j;
        } 
        else if (i <= j) 
        {
            --j;
        } 
        else 
        {
            --i;
        }
        warpPath.add(i, j);
    }
    return warpPath;
};

/* calculates both the cost and the warp path*/
template<typename T>
warpInfo<T> dtw<T>::dynamicTimeWarp(const std::vector<std::vector<T> > &seriesX, const std::vector<std::vector<T> > &seriesY)
{
    warpInfo<T> info;
    //calculate cost matrix
    info.cost = getCost(seriesX, seriesY);
    info.path = calculatePath(seriesX.size(), seriesY.size());
    return info;
}

/* calculates warp info based on window */
template<typename T>
warpInfo<T> dtw<T>::constrainedDTW(const std::vector<std::vector<T> > &seriesX, const std::vector<std::vector<T> > &seriesY, searchWindow<T> window)
{
    //initialize cost matrix
    costMatrix.clear();
    for (std::size_t i = 0; i < seriesX.size(); ++i)  //TODO: this could be smaller, since most cells are unused
    {
        std::vector<T> tempVector;
        for (std::size_t j = 0; j < seriesY.size(); ++j)
        {
            tempVector.push_back(std::numeric_limits<T>::max());
        }
        costMatrix.push_back(tempVector);
    }
    std::size_t maxX = seriesX.size() - 1;
    std::size_t maxY = seriesY.size() - 1;
    
    //fill cost matrix cells based on window
    for (std::size_t currentX = 0; currentX < window.minMaxValues.size(); ++currentX)
    {
        for (std::size_t currentY = window.minMaxValues[currentX].first; currentY <= window.minMaxValues[currentX].second; ++currentY) //FIXME: should be <= ?
        {
            if (currentX == 0 && currentY == 0) { //bottom left cell
                costMatrix[0][0] = distanceFunction(seriesX[0], seriesY[0]);
            } else if (currentX == 0) { //first column
                costMatrix[0][currentY] = distanceFunction(seriesX[0], seriesY[currentY]) + costMatrix[0][currentY - 1];
            } else if (currentY == 0) { //first row
                costMatrix[currentX][0] = distanceFunction(seriesX[currentX], seriesY[0]) + costMatrix[currentX - 1][0];
            } else {
                T minGlobalCost = fmin(costMatrix[currentX - 1][currentY], fmin(costMatrix[currentX-1][currentY-1], costMatrix[currentX][currentY-1]));
                costMatrix[currentX][currentY] = distanceFunction(seriesX[currentX], seriesY[currentY]) + minGlobalCost;
            }
        }
    }
    warpInfo<T> info;
    info.cost = costMatrix[maxX][maxY];
    info.path = calculatePath(seriesX.size(), seriesY.size());
    return info;
}

//explicit instantiation
template class dtw<double>;
template class dtw<float>;
