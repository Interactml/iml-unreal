/**
 * @file baseModel.h
 * RapidLib
 *
 * @author Michael Zbyszynski
 * @date 5 Sep 2016
 * @copyright Copyright © 2016 Goldsmiths. All rights reserved.
 */


#ifndef baseModel_h
#define baseModel_h

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
    
    inline long double strtold(const char * str, char ** str_end)
    {
        return strtod(str, str_end);
    }
}
#endif


/** Base class for wekinator models. Implemented by NN and KNN classes */
template<typename T>
class baseModel {
public:
    virtual ~baseModel() {};
    virtual T run(const std::vector<T> &inputVector) = 0;
    virtual void train(const std::vector<trainingExampleTemplate<T> > &trainingSet) = 0;
    virtual void reset() = 0;;
    virtual int getNumInputs() const = 0;
    virtual std::vector<int> getWhichInputs() const = 0;
    
#ifndef EMSCRIPTEN
    virtual void getJSONDescription(Json::Value &currentModel) = 0;
    
protected:
    template<typename TT>
    Json::Value vector2json(TT vec) {
        Json::Value toReturn;
        for (int i = 0; i < vec.size(); ++i) {
            toReturn.append(vec[i]);
        }
        return toReturn;
    }
#endif
    
};
#endif
