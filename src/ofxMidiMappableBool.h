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
        ofLogNotice("ofxMidiMappableBool")<<"map "<<value;
//        TODO: mapping: 0 -> false, !0 -> true ?
//        if(value != 0)
//        {
//            _parameter->set(true);
//        }
//        else
//        {
//            _parameter->set(false);
//        }
        _parameter->set(!_parameter->get());
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
