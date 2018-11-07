#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableInt : public ofxMidiMappable {
public:
    ofxMidiMappableInt(ofParameter<int> & parameter, int id) : ofxMidiMappable(id){
        _parameter = parameter;
        parameter.addListener(this, &ofxMidiMappableInt::notify);
    }
    void map(int value){
        _parameter = ofMap(value, 0, 127, _parameter.getMin(), _parameter.getMax());
    }
    
protected:
    void notify(int & value){
        _mapEvent.notify(_id);
    }
protected:
    ofParameter<int> _parameter;
    
private:
};

