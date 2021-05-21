/**
 * @file    searchWindow.h
 * RapidLib
 * @author  Michael Zbyszynski
 * @date    14 Sep 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#ifndef searchWindow_h
#define searchWindow_h

#include <vector>
#include "warpPath.h"

/** For fastDTW, a window of cells to calculate costs for.
 * These are cells within a specified search radius of a lower resolution path.
 */
template<typename T>
class searchWindow {
public:
    searchWindow(const int seriesXSize,
                 const int seriesYSize,
                 const warpPath &shrunkenWarpPath,
                 const int searchRadius);
    
     std::vector< std::pair<int, int> > minMaxValues;
    
private:
    int maxY;
    inline void markVisited(int col, int row);
    void expandWindow(int searchRadius);
};

#endif /* searchWindow_h */
