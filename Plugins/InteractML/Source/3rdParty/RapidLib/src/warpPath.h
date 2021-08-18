/**
 * @file  warpPath.h
 * RapidLib
 * @author Michael Zbyszynski
 * @date 13 Sep 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#pragma once

#include <vector>

/** Class to hold warp paths through a cost matrix */
class warpPath 
{
public:
    warpPath();
    ~warpPath();

    /**
     * Add a point to the beginning of the warp path.
     * (Paths are calculated starting from the end)
     */
    void add(std::size_t x, std::size_t y);
    
    std::vector< std::pair<std::size_t, std::size_t> > indices;
};

/** return struct holding a warp path and the cost of that path */
template<typename T>
struct warpInfo 
{
public:
    warpPath path;
    T cost;
    
};
