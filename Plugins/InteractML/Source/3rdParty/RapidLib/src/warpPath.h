/**
 * @file  warpPath.h
 * RapidLib
 * @author Michael Zbyszynski
 * @date 13 Sep 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#ifndef warpPath_h
#define warpPath_h

#include <vector>

/** Class to hold warp paths through a cost matrix */
class warpPath {
public:
    warpPath();
    ~warpPath();
    /**
     * Add a point to the beginning of the warp path.
     * (Paths are calculated starting from the end)
     */
    void add(int x, int y);
    
    std::vector< std::pair<int, int> > indices;
};

/** return struct holding a warp path and the cost of that path */
struct warpInfo {
public:
    warpPath path;
    double cost;
    
};

#endif /* warpPath_h */
