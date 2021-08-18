/**
 * @file    searchWindow.h
 * RapidLib
 * @author  Michael Zbyszynski
 * @date    14 Sep 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#pragma once

#include <vector>
#include "warpPath.h"

/** For fastDTW, a window of cells to calculate costs for.
 * These are cells within a specified search radius of a lower resolution path.
 */
template<typename T>
class searchWindow
{
public:
    searchWindow(const std::size_t seriesXSize,
                 const std::size_t seriesYSize,
                 const warpPath &shrunkenWarpPath,
                 const int searchRadius);
    
     std::vector< std::pair<std::size_t, std::size_t> > minMaxValues;
    
private:
    std::size_t maxY;
    inline void markVisited(std::size_t col, std::size_t row);
    void expandWindow(int searchRadius);
};
