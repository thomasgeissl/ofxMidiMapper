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

bool ofxMidiMapper::addMapping(int channel, int pitch, bool isCC, string name, bool force)
{
    if(doesMappingExist(channel, pitch, isCC))
    {
        ofLogWarning("ofxMidiMapper")<<"mapping does already exist: ("<<channel<<", "<<name<<")";
        if(force)
        {
            _mapping[tuple<int, int, bool>(channel, pitch, isCC)] = name;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        _mapping[std::tuple<int, int, bool>(channel, pitch, isCC)] = name;
        return true;
    }
}
bool ofxMidiMapper::removeMapping(int channel, int pitch, bool isCC)
{
    if(doesMappingExist(channel, pitch, isCC))
    {
        _mapping.erase(_mapping.find(std::tuple<int, int, bool>(channel, pitch, isCC)));
        return true;
    }
    else
    {
        ofLogWarning("ofxMidiMapper")<<"mapping does not exist. are you sure you are doing right?";
        return false;
    }
}

bool ofxMidiMapper::doesMappingExist(int channel, int pitch, bool isCC)
{
    return (_mapping.find(std::tuple<int, int, bool>(channel, pitch, isCC)) != _mapping.end());
}

bool ofxMidiMapper::doesMappableExist(string name)
{
    return (_mappables.find(name) != _mappables.end());
}

std::string ofxMidiMapper::getMappedName(int channel, int pitch, bool isCC)
{
    if(doesMappingExist(channel, pitch, isCC)){
        return _mapping[std::tuple<int, int, bool>(channel, pitch, isCC)];
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
        bool isCC = false;
        if(_activeMappingParameter){
            if(_nameOfMappable != "")
            {
                addMapping(channel, pitch, isCC, _nameOfMappable);
            }
            _nameOfMappable = "";
        }
        ofLogNotice("ofxMidiMapper")<<"note on: pitch = "<<pitch<<", velocity = "<<velocity;
        if(doesMappingExist(channel, pitch, isCC)){
            std::string name = getMappedName(channel, pitch, isCC);
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
        bool isCC = false;

        ofLogNotice("ofxMidiMapper")<<"note on: pitch = "<<pitch<<", velocity = "<<velocity;
        if(doesMappingExist(channel, pitch, isCC)){
            std::string name = getMappedName(channel, pitch, isCC);
        }
        break;
    }

    case MIDI_CONTROL_CHANGE:
    {
        int channel = msg.channel;
        int value = msg.value;
        int control = msg.control;
        bool isCC = true;
        ofLogNotice("ofxMidiMapper")<<"program change on: value = "<<value<<", control = "<<control;
        if(_activeMappingParameter){
            if(_nameOfMappable != "")
            {
                addMapping(channel, control, isCC, _nameOfMappable);
            }
            _nameOfMappable = "";
        }
        if(doesMappingExist(channel, control, isCC)){
            std::string name = getMappedName(channel, control, isCC);
            if(doesMappableExist(name))
            {
                getMappable(name)->map(value);
            }
        }
        else
        {
            ofLogNotice("ofxMidiMapper")<<"mapping does not already exist";
        }
        break;
    }
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

bool ofxMidiMapper::loadMapping(string path)
{
    ofFile mappingFile;
    ofJson mappingJson;
    mappingFile.open(ofToDataPath(path));
    if(mappingFile.exists())
    {
        mappingJson << mappingFile;
        ofLogNotice("ofxMidiMapper")<<"successfully loaded mappings "<< mappingJson.dump(4);
        _mapping.clear();

        for(auto mapping : mappingJson["mappings"])
        {
            addMapping(mapping["channel"].get<int>(), mapping["pitchOrCC"].get<int>(), mapping["isCC"].get<bool>(), mapping["name"].get<string>());
        }
    }
    else
    {
        ofLogError("ofxMidiMapper")<<ofToDataPath(path)<<" does not exist";
        return false;
    }
}

bool ofxMidiMapper::saveMapping(string path, bool force)
{
    ofFile mappingFile;
    mappingFile.open(ofToDataPath(path), ofFile::ReadWrite);
    if(mappingFile.exists() && !force)
    {
        ofLogError("ofxMidiMapper")<<ofToDataPath(path)<<" already exists.";
        return false;
    }
    else
    {
        mappingFile.create();
    }
    ofJson mappingJson;

    mappingJson["mappings"] = ofJson::array();
    for(auto mapping : _mapping)
    {
        int channel = std::get<0>(mapping.first);
        int pitchOrCC = std::get<1>(mapping.first);
        bool isCC = std::get<2>(mapping.first);

        std::string name = mapping.second;
        mappingJson["mappings"].push_back({{"channel", channel}, {"pitchOrCC", pitchOrCC}, {"isCC", isCC}, {"name", name}});
    }
    mappingFile << mappingJson.dump(4);
    mappingFile.close();
    return true;
}
