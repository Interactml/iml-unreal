//
// filter_utilities.cpp
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

#include "filter_utilities.h"

void filtfilt(vector<double> const& b, vector<double> const& a, vector<double> & x, vector<double> & y, PADTYPE padtype, int padlen)
{
    int ntaps = int(max(a.size(), b.size()));
    
    if (padtype == NONE)
        padlen=0;
    unsigned int edge;
    if (padlen < 0)
        edge = ntaps * 3;
    else
        edge = padlen;
    
    if (x.size() <= edge)
        throw runtime_error("The length of the input vector x must be at least padlen.");
    
    vector<double> ext;
    if (padtype != NONE && edge > 0) {
        // Make an extension of length 'edge' at each
        // end of the input array.
        switch (padtype) {
            case EVEN:
                even_ext<double>(x, ext, edge);
                break;
            case ODD:
                odd_ext<double>(x, ext, edge);
                break;
            default:
                const_ext<double>(x, ext, edge);
        }
    } else {
        ext = x;
    }
    
    // Get the steady state of the filter's step response.
    vector<double> zi;
    lfilter_zi(b, a, zi);
    
    // Forward filter.
    y.resize(ext.size());
    vector<double> zip = zi;
    for (unsigned int i=0 ; i<zi.size(); i++) {
        zip[i] = zi[i] * x[0];
    }
    
    vector<double> y_reverse(ext.size());
    lfilter(b, a, ext, y_reverse, zip);
    
    reverse_copy(y_reverse.begin(), y_reverse.end(), y.begin());
    
    // Backward filter.
    // Create y0 so zi*y0 broadcasts appropriately.
    for (unsigned int i=0 ; i<zip.size(); i++) {
        zip[i] = zi[i] * x[x.size()-1];
    }
    lfilter(b, a, y, y_reverse, zip);
    
    // Reverse y.
    y.resize(x.size());
    reverse_copy(y_reverse.begin()+edge, y_reverse.end()-edge, y.begin());
}


void lfilter(vector<double> const& b, vector<double> const& a, vector<double> const& x, vector<double> & y, vector<double> const& zi)
{
    vector<double> _b = b;
    vector<double> _a = a;
    
    // Pad a or b with zeros so they are the same length.
    unsigned int k = int (max(a.size(), b.size()));
    
    if (_a.size() < k)
        _a.resize(k, 0.);
    else if (_b.size() < k)
        _b.resize(k, 0.);
    
    if (_a[0] != 1.0) {
        // Normalize the coefficients so a[0] == 1.
        for (unsigned int i=0; i<k; i++) {
            _a[i] /= _a[0];
            _b[i] /= _a[0];
        }
    }
    
    vector<double> z = zi;
    unsigned int n = int(x.size());
    y.resize(n);
    for (unsigned int m=0; m<n; m++) {
        y[m] = _b[0] * x[m] + z[0];
        for (unsigned int i=0; i<k-2; i++) {
            z[i] = _b[i+1] * x[m] + z[i+1] - _a[i+1] * y[m];
        }
        z[k-2] = _b[k-1] * x[m] - _a[k-1] * y[m];
    }
}


void lfilter_zi(vector<double> const& b, vector<double> const& a, vector<double> & zi) {
    vector<double> _b = b;
    vector<double> _a = a;
    
    if (_a[0] != 1.0) {
        // Normalize the coefficients so a[0] == 1.
        for (unsigned int i=0; i<_a.size(); i++) {
            _a[i] /= _a[0];
            _b[i] /= _a[0];
        }
    }
    
    unsigned int n = int (max(_a.size(), _b.size()));
    
    // Pad a or b with zeros so they are the same length.
    if (_a.size() < n)
        _a.resize(n, 0.);
    else if (_b.size() < n)
        _b.resize(n, 0.);
    
    vector<double> IminusA((n-1)*(n-1), 0.);
    for (unsigned int i=0; i<n-1; i++) {
        IminusA[i*(n-1)+i] = 1.;
    }
    
    vector<double> companion((n-1)*(n-1), 0.);
    for (unsigned int i=0; i<n-1; i++) {
        companion[i] = -_a[i+1] / (1.0 * _a[0]);
    }
    for (unsigned int i=1; i<n-1; i++) {
        companion[i*(n-1)+i-1] = 1.;
    }
    
    for (unsigned int i=0; i<n-1; i++) {
        for (unsigned int j=0; j<n-1; j++) {
            IminusA[i*(n-1)+j] -= companion[j*(n-1)+i];
        }
    }
	
    zi.clear();
    zi.resize(n - 1, 0.);
    
    double tmpSum(0.);
    for (unsigned int i=0; i<n-1; i++) {
        zi[0] += _b[i+1] - _a[i+1] * _b[0];
        tmpSum += IminusA[i*(n-1)];
    }
    zi[0] /= tmpSum;
    
    // TODO: remove B
    
    // Solve zi = A*zi + B
    double asum = 1.0;
    double csum = 0.0;
    for (unsigned int k=1; k<n-1; k++) {
        asum += _a[k];
        csum += _b[k] - _a[k]*_b[0];
        zi[k] = asum*zi[0] - csum;
    }
}

