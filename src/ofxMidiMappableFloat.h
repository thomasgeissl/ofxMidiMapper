#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableFloat : public ofxMidiMappable {
public:
    ofxMidiMappableFloat(ofParameter<float> & parameter, int id, bool catchCurrentValue) : ofxMidiMappable(id), _parameter(parameter), _catchCurrentValue(catchCurrentValue){
        parameter.addListener(this, &ofxMidiMappableFloat::notify);
    }
    void map(int value){
        auto newValue = ofMap(value, 0, 127, _parameter.getMin(), _parameter.getMax());
        if(_catchCurrentValue){
            if(std::abs(newValue - _parameter) < (_parameter.getMax() - _parameter.getMin())*.05){
                _parameter = newValue;
            }
        }else{
            _parameter = newValue;
        }
    }
    void notify(float & value){
        _mapEvent.notify(_id);
    }
    ofParameter<float> _parameter;
    bool _catchCurrentValue;
};
