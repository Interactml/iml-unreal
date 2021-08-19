/**
 * @file warpPath.cpp
 * RapidLib
 * @author Michael Zbyszysnski
 * @date 13 Sep 2017
 *
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#include "warpPath.h"

warpPath::warpPath() {};
warpPath::~warpPath() {};

void warpPath::add(std::size_t x, std::size_t y)
{
    indices.insert(indices.begin(), std::make_pair(x, y));
};
