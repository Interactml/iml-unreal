//
// filter_utilities.h
//
// Filtering utilities
// >> c++ implementations of scipy.signal standard filtering functions
//
// Copyright (C) 2013-2014 by IRCAM - Centre Pompidou. All Rights Reserved.
// author: Jules Francoise
// contact: jules.francoise@ircam.fr
//
// created 2013-12-24
//

#ifndef __emg_bayesfilter__filter_utilities__
#define __emg_bayesfilter__filter_utilities__

#include <algorithm>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>

using namespace std;

typedef enum _padtype {
    EVEN,
    ODD,
    CONSTANT,
    NONE
} PADTYPE;

void filtfilt(vector<double> const& b, vector<double> const& a, vector<double> & x, vector<double> & y, PADTYPE padtype = ODD, int padlen=-1);
void lfilter_zi(vector<double> const& b, vector<double> const& a, vector<double> & zi);
void lfilter(vector<double> const& b, vector<double> const& a, vector<double> const& x, vector<double> & y, vector<double> const& zi);

/*! 
 1D python-like even_ext function.
 
 */
template <typename datatype>
void even_ext(vector<datatype> const& src, vector<datatype> & dst, unsigned int n)
{
    if (n<1)
        dst = src;
    if (n > src.size() - 1)
        throw runtime_error("The extension length n is too big. It must not exceed src.size()-1.");
    
    dst.resize(2 * n + src.size());
    
    int t(0);
    for (int i=n; i>0; i--) {
        dst[t++] = src[i];
    }
    copy(src.begin(), src.end(), dst.begin()+n);
    
    t += src.size();
    for (unsigned int i =  int (src.size()-2); i>src.size()-n-2; i--) {
        dst[t++] = src[i];
    }
}

// 1D python-like odd_ext
template <typename datatype>
void odd_ext(vector<datatype> const& src, vector<datatype> & dst, unsigned int n)
{
    if (n<1)
        dst = src;
    if (n > src.size() - 1)
        throw runtime_error("The extension length n is too big. It must not exceed src.size()-1.");
    
    dst.resize(2 * n + src.size());
    
    int t(0);
    for (int i=n; i>0; i--) {
        dst[t++] = 2 * src[0] - src[i];
    }
    copy(src.begin(), src.end(), dst.begin()+n);
    
    t += src.size();
    for (unsigned int i = int (src.size() - 2); i>src.size()-n-2; i--) {
        dst[t++] = 2 * src[src.size()-1] - src[i];
    }
}

// 1D python-like const_ext
template <typename datatype>
void const_ext(vector<datatype> const& src, vector<datatype> & dst, unsigned int n)
{
    if (n<1)
        dst = src;
    if (n > src.size() - 1)
        throw runtime_error("The extension length n is too big. It must not exceed src.size()-1.");
    
    dst.resize(2 * n + src.size());
    
    int t(0);
    for (int i=n; i>0; i--) {
        dst[t++] = src[0];
    }
    copy(src.begin(), src.end(), dst.begin()+n);
    
    t += src.size();
    for (unsigned int i = int (src.size() - 2); i>src.size()-n-2; i--) {
        dst[t++] = src[src.size()-1];
    }
}


#endif

