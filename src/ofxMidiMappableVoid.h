#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableVoid : public ofxMidiMappable {
public:
    ofxMidiMappableVoid(ofParameter <void> * parameter, int id) : ofxMidiMappable(id){
        _parameter = parameter;
        parameter->addListener(this, &ofxMidiMappableVoid::notify);
    }
    
    void map(int value){
        //            _parameter->set(ofMap(value, 0, 127, _parameter->getMin(), _parameter->getMax()));
    }
    
protected:
    void notify(){
        _mapEvent.notify(_id);
    }
protected:
    ofParameter <void> * _parameter;
    
private:
};

