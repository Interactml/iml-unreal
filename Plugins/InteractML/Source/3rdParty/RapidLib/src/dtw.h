/**
 * @file dtw.h
 * RapidLib
 *
 * @author Michael Zbyszynski
 * @date 07 Jun 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#pragma once

#include <vector>
#include "warpPath.h"
#include "searchWindow.h"

template<typename T>
class dtw 
{
public:
    dtw();
    ~dtw();
    
    /* Calculates and returns a simple cost value between two input series */
    T getCost(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY);
    
    /* Calculates both cost and the warp path */
    warpInfo<T> dynamicTimeWarp(const std::vector<std::vector<T> > &seriesX, const std::vector<std::vector<T> > &seriesY); //This returns everything, including a path
    
    /* Calculates both the cost and the warp path, with a given window as a constraint */
    warpInfo<T> constrainedDTW(const std::vector<std::vector<T> > &seriesX, const std::vector<std::vector<T> > &seriesY, searchWindow<T> window); //This takes a window object
    
private:
    inline T distanceFunction(const std::vector<T> &pointX, const std::vector<T> &point);
    std::vector<std::vector<T> > costMatrix;
    warpPath calculatePath(std::size_t seriesXsize, std::size_t seriesYsize) const;
};
