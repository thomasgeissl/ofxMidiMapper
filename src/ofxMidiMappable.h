#pragma once
#include "ofParameter.h"

class ofxMidiMapper;
class ofxMidiMappable {
public:
    void addListener(ofxMidiMapper & listener);
    virtual void map(int value){
    }
    ofEvent <std::string> & getMapEvent();
    
protected:
    ofEvent <std::string> _mapEvent;
    
private:
};

