/**
 * @file    rapidLib.h
 * @author  Michael Zbyszynski
 * @date    24 April 2020
 *
 * @copyright
 * Copyright © 2020 Michael Zbyszynski. All rights reserved.
 */


//                  _     _ _      _ _
//                 (_)   | | |    (_) |
//  _ __ __ _ _ __  _  __| | |     _| |__
// | '__/ _` | '_ \| |/ _` | |    | | '_ \ 
// | | | (_| | |_) | | (_| | |____| | |_) |
// |_|  \__,_| .__/|_|\__,_|______|_|_.__/
//           | |
//           |_|

 /**
  * @mainpage RapidLib
  *
  * @section Introduction
  * 
  * RapidLib is a lightweight library for interactive machine learning, inspired by Wekinator. It currently features classification (using kNN), regression (multilayer perceptron), and series classification (using dynamic time warping).
  *
  * All the classes, functions and structs documented here belong to the
  * <code>rapidlib</code> namespace.
  *
  * @section Examples
  * @subsection Simple classification
  * TODO
  */

#ifndef rapidlib_h
#define rapidlib_h

#define RAPIDLIB_VERSION_MAJOR 2
#define RAPIDLIB_VERSION_MINOR 2
#define RAPIDLIB_VERSION_PATCH 0
#define RAPIDLIB_VERSION "2.2.0"
#define RAPIDLIB_JSON_DOC_VERSION "1.0.0"
#define RAPIDLIB_REVISION "15-June-2021"

#include "regression.h"
#include "classification.h"
#include "seriesClassification.h"
#include "rapidStream.h"

#endif