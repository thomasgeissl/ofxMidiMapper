#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableBool : public ofxMidiMappable
{
public:
    void setup(ofParameter<bool> *parameter)
    {
        ofLogNotice("ofxMidiMappableBool")<<"setup";
        _parameter = parameter;
        parameter->addListener(this, &ofxMidiMappableBool::notify);
    }
    void map(int value)
    {
        ofLogNotice("ofxMidiMappableFloat")<<"map "<<value;
//        TODO: mapping: 0 -> false, !0 -> true ?
//        _parameter->set(ofMap(value, 0, 127, _parameter->getMin(), _parameter->getMax()));
    }

protected:
    void notify(bool &value)
    {
        string name = _parameter->getName();
        ofLogNotice("ofxMidiMappableBool")<<"notify "<<name;
        _mapEvent.notify(name);
    }
protected:
    ofParameter<bool> *_parameter;

private:
};
