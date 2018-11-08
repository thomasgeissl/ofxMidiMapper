#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableVoid : public ofxMidiMappable {
public:
    ofxMidiMappableVoid(ofParameter <void> & parameter, int id) : ofxMidiMappable(id), _parameter(parameter){
        parameter.addListener(this, &ofxMidiMappableVoid::notify);
    }
    void map(int value){
        _parameter.trigger();
    }
    void notify(){
        _mapEvent.notify(_id);
    }
    ofParameter <void> _parameter;
};
