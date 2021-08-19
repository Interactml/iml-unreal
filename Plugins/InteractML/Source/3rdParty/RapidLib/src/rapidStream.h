/**
 * @file    rapidStream.h
 * @author  Michael Zbyszynski
 * @date    6 Feb 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#ifndef rapidStream_h
#define rapidStream_h

#include <stdint.h>
#include <atomic>
#include "../dependencies/bayesfilter/src/BayesianFilter.h"

namespace rapidLib
{
    template<typename T>
    class rapidStream
    {
    public:

        /**
         * Create a circular buffer with 3 elements.
         */
        rapidStream();
        /**
         * Create a circular buffer with an arbitrary number of elements.
         * @param int: number of elements to hold in the buffer
         */
        rapidStream(std::size_t windowSize);

        ~rapidStream();

        /**
         * Resets all the values in the buffer to zero.
         */
        void clear();

        /** Add a value to a circular buffer whose size is defined at creation.
         * @param double: value to be pushed into circular buffer.
         */
        void pushToWindow(T input);

        /** Calculate the first-order difference (aka velocity) between the last two inputs.
         * @return double: difference between last two inputs.
         */
        T velocity() const;

        /** Calculate the second-order difference (aka acceleration) over the last three inputs.
         * @return double: acceleration over the last three inputs.
         */
        T acceleration() const;

        /** Find the minimum value in the buffer.
         * @return double: minimum.
         */
        T minimum() const;

        /** Find the maximum value in the buffer.
         * @return double: maximum.
         */
        T maximum() const;

        /** Count the number of zero crossings in the buffer.
         * @return int: number of zero crossings.
         */
        uint32_t numZeroCrossings() const;

        /** Calculate the sum of all values in the buffer.
         * @return T: sum.
         */
        T sum() const;

        /** Calculate the mean of all values in the buffer.
         * @return double: mean.
         */
        T mean() const;

        /** Calculate the standard deviation of all values in the buffer.
         * @return double: standard deviation.
         */
        T standardDeviation() const;

        /** Calculate the root mean square of the values in the buffer
         * @return double: rms
         */
        T rms() const;

        /** Non-linear Baysian filtering for EMG envelope extraction.
         * @return current envelope value
         */
        T bayesFilter(T inputValue);
        void bayesSetDiffusion(float logDiffusion);
        void bayesSetJumpRate(float jump_rate);
        void bayesSetMVC(float mvc);

        /** Calculate the minimum first-order difference over consecutive inputs in the buffer.
         * @return double: minimum velocity.
         */
        T minVelocity() const;

        /** Calculate the maximum first-order difference over consecutive inputs in the buffer.
         * @return double: maximum velocity.
         */
        T maxVelocity() const;

        /** Calculate the minimum second-order difference over consecutive inputs in the buffer.
         * @return double: minimum acceleration.
         */
        T minAcceleration() const;

        /** Calculate the maximum second-order difference over consecutive inputs in the buffer.
         * @return double: maximum acceleration.
         */
        T maxAcceleration() const;

    private:
        std::size_t windowSize;
        std::atomic<uint32_t> windowIndex;
        T* circularWindow;

        inline T calcCurrentVel(std::size_t i) const;

        BayesianFilter bayesFilt;
    };
};

#endif
