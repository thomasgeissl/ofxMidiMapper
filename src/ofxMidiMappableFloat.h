#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableFloat : public ofxMidiMappable {
public:
    ofxMidiMappableFloat(ofParameter<float> & parameter, int id) : ofxMidiMappable(id), _parameter(parameter){
        parameter.addListener(this, &ofxMidiMappableFloat::notify);
    }
    void map(int value){
        _parameter = ofMap(value, 0, 127, _parameter.getMin(), _parameter.getMax());
    }
    void notify(float & value){
        _mapEvent.notify(_id);
    }
    ofParameter<float> _parameter;
};
