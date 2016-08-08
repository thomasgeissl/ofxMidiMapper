#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableFloat : public ofxMidiMappable
{
public:
    void setup(ofParameter<float> *parameter)
    {
        ofLogNotice("ofxMidiMappableFloat")<<"setup";
        _parameter = parameter;
        parameter->addListener(this, &ofxMidiMappableFloat::notify);
    }
    void map(int value)
    {
        ofLogNotice("ofxMidiMappableFloat")<<"map "<<value;
        _parameter->set(ofMap(value, 0, 127, _parameter->getMin(), _parameter->getMax()));
    }

protected:
    void notify(float &value)
    {
        string name = _parameter->getName();
        ofLogNotice("ofxMidiMappableFloat")<<"notify "<<name;
        _mapEvent.notify(name);
    }
protected:
    ofParameter<float> *_parameter;

private:
};
