#pragma once
#include "ofParameter.h"

class ofxMidiMapper;
class ofxMidiMappable {
public:
    ofxMidiMappable(int id);
    void addListener(ofxMidiMapper & listener);
    virtual void map(int value){}
    ofEvent<int> & getMapEvent();
    
protected:
    ofEvent<int> _mapEvent;
    int _id;
    
private:
};

