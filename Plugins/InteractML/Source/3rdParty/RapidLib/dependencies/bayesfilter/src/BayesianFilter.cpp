//
// BayesianFilter.cpp
//
// Non-linear Baysian filtering for EMG Enveloppe Extraction.
// Based on Matlab code by Terence Sanger : kidsmove.org/bayesemgdemo.html
//
// Reference:
// - Sanger, T. (2007). Bayesian filtering of myoelectric signals. Journal of neurophysiology, 1839–1845.
//
//
// Copyright (C) 2013-2014 by IRCAM - Centre Pompidou. All Rights Reserved.
// author: Jules Francoise
// contact: jules.francoise@ircam.fr
//
// created 2013-12-24
//

#include "BayesianFilter.h"
#include "filter_utilities.h"


#pragma mark -
#pragma mark Constructors
BayesianFilter::BayesianFilter()
{
    mvc.assign(channels, 1.);
    init();
}

BayesianFilter::~BayesianFilter()
{
    
}

void BayesianFilter::resize(std::size_t size)
{
    if (size > 0) {
        channels = size;
        init();
    }
}

std::size_t BayesianFilter::size() const
{
    return channels;
}

#pragma mark -
#pragma mark Main Algorithm
void BayesianFilter::init()
{
    mvc.resize(channels, 1.);
    output.assign(channels, 0.);
    prior.resize(channels);
    state.resize(channels);
    g.resize(channels);
    for (unsigned int i=0; i<channels; i++) {
        prior[i].resize(levels);
        state[i].resize(levels);
        g[i].resize(3);
        
        double val(1.);
        for (unsigned int t=0; t<levels; t++) {
            state[i][t] = val * mvc[i] / double(levels);
            val += 1;
            prior[i][t] = 1. / levels;
        }
        
        double diff = diffusion * diffusion / (samplerate * std::pow(mvc[i] / levels, 2));
        g[i][0] = diff / 2.;
        g[i][1] = 1. - diff - this->jump_rate;
        g[i][2] = diff / 2.;
    }
}

void BayesianFilter::update(vector<float> const& observation)
{
    if (observation.size() != this->channels) {
        resize(observation.size());
    }
    
    for (std::size_t i=0; i<channels; i++)
    {
        // -- 1. Propagate
        // -----------------------------------------
        
        vector<double> a(1, 1.);
        vector<double> oldPrior(prior[i].size());
        //        oldPrior.swap(prior[i]);
        copy(prior[i].begin(), prior[i].end(), oldPrior.begin());
        
        filtfilt(g[i], a, oldPrior, prior[i]);
        
        // set probability of a sudden jump
        for (unsigned int t=0; t<levels; t++) {
            prior[i][t] = prior[i][t] + jump_rate / mvc[i];
        }
        
        // -- 4. Calculate the posterior likelihood function
        // -----------------------------------------
        // calculate posterior density using Bayes rule
        vector<double> posterior(levels);
        double sum_posterior(0.);
        for (unsigned int t=0; t<levels; t++) {
            double x_2 = state[i][t] * state[i][t];
            posterior[t] = this->prior[i][t] * exp(- observation[i] * observation[i] / x_2) / x_2;
            sum_posterior += posterior[t];
        }
        
        // -- 5. Output the signal estimate output(x(t)) = argmax P(x,t);
        // -----------------------------------------
        // find the maximum of the posterior density
        unsigned int pp(0);
        double tmpMax(posterior[0]);
        for (unsigned int t=0; t<levels; t++) {
            if (posterior[t] > tmpMax) {
                tmpMax = posterior[t];
                pp = t;
            }
            posterior[t] /= sum_posterior;
        }
        
        // convert index of peak value to scaled EMG value
        output[i] = state[i][pp] / mvc[i];
        
        // -- 7. Repeat from step 2 > prior for next iteration is posterior from this iteration
        // -----------------------------------------
        copy(posterior.begin(), posterior.end(), prior[i].begin());
    }
}
