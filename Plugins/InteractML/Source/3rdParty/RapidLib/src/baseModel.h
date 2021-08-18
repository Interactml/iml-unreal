/**
 * @file baseModel.h
 * RapidLib
 *
 * @author Michael Zbyszynski
 * @date 5 Sep 2016
 * @copyright Copyright © 2016 Goldsmiths. All rights reserved.
 */

#pragma once

#include <vector>
#include "trainingExample.h"

#ifndef EMSCRIPTEN
#include "../dependencies/json/json.h"
#endif

#ifdef __ANDROID__
#include <ostream>
namespace std
{ // Workaround for android
    template <typename T>
    std::string to_string(T Value)
    {
        std::ostringstream TempStream;
        TempStream << Value;
        return TempStream.str();
    }

    inline long double strtold(const char* str, char** str_end)
    {
        return strtod(str, str_end);
    }
}
#endif

/** Base class for wekinator models. Implemented by NN and KNN classes */
template<typename T>
class baseModel
{
public:
    virtual ~baseModel() {};
    virtual T run(const std::vector<T>& inputVector) = 0; //TODO: I'd like this to be const
    virtual void train(const std::vector<trainingExampleTemplate<T> >& trainingSet) = 0;
    virtual void train(const std::vector<trainingExampleTemplate<T> >& trainingSet, const std::size_t whichOutput) = 0;
    virtual void reset() = 0;;
    virtual size_t getNumInputs() const = 0;
    virtual std::vector<size_t> getWhichInputs() const = 0;

#ifndef EMSCRIPTEN
    virtual void getJSONDescription(Json::Value& currentModel) = 0;

protected:
    
    template<typename TT, class Dummy=int>
    Json::Value vector2json(TT vec) 
    {
        Json::Value toReturn;
        for (size_t i = 0; i < vec.size(); ++i) 
        {
            toReturn.append( (Json::Value)vec[i] );
        }
        return toReturn;
    }

    //FIXME: This is a temporary hack because Json::Value doesn't know what to do with unsinged longs, and XCode cares
    template<class Dummy=int>
    Json::Value vector2json(std::vector<unsigned long> vec)
    {
        Json::Value toReturn;
        for (size_t i = 0; i < vec.size(); ++i)
        {
            toReturn.append((double)vec[i]); //I chose double here because that's close to what JS uses
        }
        return toReturn;
    }
#endif

};
