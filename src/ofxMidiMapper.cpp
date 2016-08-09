#include "ofxMidiMapper.h"

ofxMidiMapper::ofxMidiMapper()
{
    _parameters.setName("parameters");
    _activeParameter.set("on/off", false);
    _activeMappingParameter.set("mapping mode", false);

    _parameters.add(_activeParameter);
    _parameters.add(_activeMappingParameter);

    _midiIn.addListener(this);
}

void ofxMidiMapper::addParameter(ofParameter<bool> *parameter)
{
    ofxMidiMappableBool *mappable = new ofxMidiMappableBool();
    mappable->setup(parameter);
//    mappable.addListener(*this);
    ofAddListener(mappable->getMapEvent(), this, &ofxMidiMapper::onMapEvent);
    _mappables[parameter->getName()] = mappable;
}
void ofxMidiMapper::addParameter(ofParameter<float> *parameter)
{
    ofxMidiMappableFloat *mappable = new ofxMidiMappableFloat();
    mappable->setup(parameter);
//    mappable.addListener(*this);
    ofAddListener(mappable->getMapEvent(), this, &ofxMidiMapper::onMapEvent);
    _mappables[parameter->getName()] = mappable;
}
void ofxMidiMapper::addParameter(ofParameter<int> *parameter)
{
    ofxMidiMappableInt *mappable = new ofxMidiMappableInt();
    mappable->setup(parameter);
//    mappable.addListener(*this);
    ofAddListener(mappable->getMapEvent(), this, &ofxMidiMapper::onMapEvent);
    _mappables[parameter->getName()] = mappable;
}

void ofxMidiMapper::openMidiPort(int port)
{
    _midiIn.closePort();
    _midiIn.openPort(port);
}

void ofxMidiMapper::openVirtualMidiPort(string name)
{
    _midiIn.closePort();
    _midiIn.openVirtualPort(name);
}

bool ofxMidiMapper::addMapping(int channel, int pitch, string name, bool force)
{
    if(doesMappingExist(channel, pitch))
    {
        ofLogWarning("ofxMidiMapper")<<"mapping does already exist: ("<<channel<<", "<<name<<")";
        if(force)
        {
            _mapping[pair<int, int>(channel, pitch)] = name;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        _mapping[pair<int, int>(channel, pitch)] = name;
        return true;
    }
}
bool ofxMidiMapper::removeMapping(int channel, int pitch)
{
    if(doesMappingExist(channel, pitch))
    {
        _mapping.erase(_mapping.find(pair<int, int>(channel, pitch)));
        return true;
    }
    else
    {
        ofLogWarning("ofxMidiMapper")<<"mapping does not exist. are you sure you are doing right?";
        return false;
    }
}

bool ofxMidiMapper::doesMappingExist(int channel, int pitch)
{
    return (_mapping.find(pair<int, int>(channel, pitch)) != _mapping.end());
}

bool ofxMidiMapper::doesMappableExist(string name)
{
    return (_mappables.find(name) != _mappables.end());
}

std::string ofxMidiMapper::getMappedName(int channel, int pitch)
{
    if(doesMappingExist(channel, pitch)){
        return _mapping[pair<int, int>(channel, pitch)];
    }else{
//        TODO: better throw an exception instead of returning an empty string?
        ofLogError("ofxMidiMapper")<<"mapping does not exist. returning empty string";
        return "";
    }
}

ofxMidiMappable *ofxMidiMapper::getMappable(string name)
{
    if(doesMappableExist(name))
    {
        return _mappables[name];
    }
    else
    {
//        TODO: better throw an exception instead of returning nullptr?
        ofLogError("ofxMidiMapper")<<"mappable does not exit. returning nullptr";
        return nullptr;
    }
}

ofParameterGroup &ofxMidiMapper::getParameters()
{
    return _parameters;
}

void ofxMidiMapper::newMidiMessage(ofxMidiMessage &msg)
{
    if(!_activeParameter) return;

    switch(msg.status)
    {
    case MIDI_NOTE_ON:
    {
        int channel = msg.channel;
        int pitch = msg.pitch;
        int velocity = msg.velocity;
        if(_activeMappingParameter){
            if(_nameOfMappable != "")
            {
                addMapping(channel, pitch, _nameOfMappable);
            }
            _nameOfMappable = "";
        }
        ofLogNotice("ofxMidiMapper")<<"note on: pitch = "<<pitch<<", velocity = "<<velocity;
        if(doesMappingExist(channel, pitch)){
            std::string name = getMappedName(channel, pitch);
            if(doesMappableExist(name))
            {
                getMappable(name)->map(velocity);
            }
        }
        else
        {
            ofLogNotice("ofxMidiMapper")<<"mapping does not already exist";
        }
        break;
    }
    case MIDI_NOTE_OFF:
    {
        int channel = msg.channel;
        int pitch = msg.pitch;
        int velocity = msg.velocity;
        ofLogNotice("ofxMidiMapper")<<"note on: pitch = "<<pitch<<", velocity = "<<velocity;
        if(doesMappingExist(channel, pitch)){
            std::string name = getMappedName(channel, pitch);
        }
        break;
    }

    case MIDI_CONTROL_CHANGE: break;
    case MIDI_PROGRAM_CHANGE: break;
    default: break;
    }
}

void ofxMidiMapper::onMapEvent(string &nameOfMappable)
{
    ofLogNotice("ofxMidiMapper")<<"on map Event "<<nameOfMappable;
    if(_activeMappingParameter){
        _nameOfMappable = nameOfMappable;
        ofLogNotice("ofxMidiMapper")<<"name of mappable "<<_nameOfMappable;
    }
}
