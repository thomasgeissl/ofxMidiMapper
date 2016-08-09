#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMidiMappable.h"
#include "ofxMidiMappableBool.h"
#include "ofxMidiMappableFloat.h"
#include "ofxMidiMappableInt.h"


class ofxMidiMapper :
        public ofxMidiListener
{
public:
    ofxMidiMapper();
    void addParameter(ofParameter<bool> *parameter);
    void addParameter(ofParameter<float> *parameter);
    void addParameter(ofParameter<int> *parameter);

    void openMidiPort(int port);
    void openVirtualMidiPort(string name);

    bool addMapping(int channel, int pitch, std::string name, bool force = true);
    bool removeMapping(int channel, int pitch);

    bool doesMappingExist(int channel, int pitch);
    bool doesMappableExist(std::string name);
    std::string getMappedName(int channel, int pitch);
    ofxMidiMappable* getMappable(std::string name);
    ofParameterGroup & getParameters();
    void onMapEvent(string &nameOfMappable);

protected:
private:
    void newMidiMessage(ofxMidiMessage &msg);

private:
    ofxMidiIn _midiIn;
    std::map<std:: string, ofxMidiMappable*> _mappables;

//    std::map<std::pair<int, int>, string> _mapping;
    std::map<std::pair<int, int>, string> _mapping;

    ofParameterGroup _parameters;
    ofParameter<bool> _activeParameter;
    ofParameter<bool> _activeMappingParameter;

    std::string _nameOfMappable;
};
