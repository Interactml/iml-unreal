/**
 * @file  fastDTW.h
 * RapidLib
 * @author Michael Zbyszynski
 * @date 07 Sep 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#pragma once

#include <vector>
#include "warpPath.h"

/** Class for performing an fast dynamic time warping between two time series*/
template<typename T>
class fastDTW
{
public:
    fastDTW();
    ~fastDTW();
    
    /**
     * Returns just the cost of warping one series into a second.
     * @param seriesX time series X
     * @param seriesY time series Y
     * @param searchRadius search radius (usually 1)
     * @return cost to warp between series
     */
    static T getCost(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius);
    
private:
    /**
     * Returns the cost and the warp path.
     * @param seriesX time series X
     * @param seriesY time series Y
     * @param searchRadius search radius (usually 1)
     * @return information about optimal time warp
     */
    static warpInfo<T> fullFastDTW(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius);

    /**
     * Returns just lowest cost path to warping one series into a second.
     * @param seriesX time series X
     * @param seriesY time series Y
     * @param searchRadius search radius (usually 1)
     * @return The warp path
     */
    static warpPath getWarpPath(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius);
    
    /**
     * Downsamples a time series by two. Resolution isn't implemented yet
     * @param series
     * @param resolution (not used)
     * @return downsampled series
     `*/
    inline static std::vector<std::vector<T> > downsample(const std::vector<std::vector<T>> &series, T resolution);
    
};
