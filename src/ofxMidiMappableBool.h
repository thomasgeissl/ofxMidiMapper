#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableBool : public ofxMidiMappable {
public:
    ofxMidiMappableBool(ofParameter <bool> & parameter, int id) : ofxMidiMappable(id), _parameter(parameter){
        _parameter.addListener(this, &ofxMidiMappableBool::notify);
    }
    void map(int value){
        //        TODO: mapping: 0 -> false, !0 -> true ?
        //        if(value != 0)
        //        {
        //            _parameter->set(true);
        //        }
        //        else
        //        {
        //            _parameter->set(false);
        //        }
        _parameter = !_parameter;
    }
    void notify(bool & value){
        _mapEvent.notify(_id);
    }
    ofParameter<bool> _parameter;
};
