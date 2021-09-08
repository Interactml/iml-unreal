/**
 * @file fastDTW.cpp
 * RapidLib
 *
 * @author Michael Zbyszynski
 * @date 07 Sep 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#include "fastDTW.h"
#include "dtw.h"

template<typename T>
fastDTW<T>::fastDTW() {};

template<typename T>
fastDTW<T>::~fastDTW() {};

template<typename T>
warpInfo<T> fastDTW<T>::fullFastDTW(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius)
{
   
#ifndef EMSCRIPTEN
    if (seriesY.size() > seriesX.size()) {
        return fullFastDTW(seriesY, seriesX, searchRadius); //TODO: I'm not sure why I need this. Also, not sure why it fails with Emscripten.
    }
#endif
    
    dtw<T> dtw;
    searchRadius = (searchRadius < 0) ? 0 : searchRadius;
    int minSeries = searchRadius + 2;
    if (seriesX.size() <= minSeries || seriesY.size() <= minSeries) {
        return dtw.dynamicTimeWarp(seriesX, seriesY);
    }
    
    T resolution = 2.0;//TODO: Just hardcode this?
    std::vector<std::vector<T>> shrunkenX = downsample(seriesX, resolution);
    std::vector<std::vector<T>> shrunkenY = downsample(seriesY, resolution);
    
    //some nice recursion here
    searchWindow<T> window(int(seriesX.size()), int(seriesY.size()), getWarpPath(shrunkenX, shrunkenY, searchRadius), searchRadius);
    return dtw.constrainedDTW(seriesX, seriesY, window);
};

template<typename T>
T fastDTW<T>::getCost(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius){
    warpInfo<T> info = fullFastDTW(seriesX, seriesY, searchRadius);
    return info.cost;
};

template<typename T>
warpPath fastDTW<T>::getWarpPath(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius){
    warpInfo<T> info = fullFastDTW(seriesX, seriesY, searchRadius);
    return info.path;
};

template<typename T>
inline std::vector<std::vector<T> > fastDTW<T>::downsample(const std::vector<std::vector<T>> &series, T resolution) {
    std::vector<std::vector<T> > shrunkenSeries;
    for (std::size_t i = 0; i < series.size(); ++i) {
        if (i % 2 == 0) {
            shrunkenSeries.push_back(series[i]);
        } else {
            int shrunkIndex = int(i * 0.5);
            for (std::size_t j = 0; j < series[i].size(); ++j) {
                shrunkenSeries[shrunkIndex][j] = (shrunkenSeries[shrunkIndex][j] + series[i][j]) * (T)0.5;
            }
        }
    }
    //TODO: implement downsampling by resolution
    return shrunkenSeries;
}

//explicit instantiation
template class fastDTW<double>;
template class fastDTW<float>;
