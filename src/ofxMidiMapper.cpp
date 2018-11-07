#include "ofxMidiMapper.h"

ofxMidiMapper::ofxMidiMapper() : _idCounter(0){
	_parameters.setName("parameters");
	_activeParameter.set("on/off", false);
	_activeMappingParameter.set("mapping mode", false);

	_parameters.add(_activeParameter);
	_parameters.add(_activeMappingParameter);

	_midiIn.addListener(this);
}

void ofxMidiMapper::addParameter(ofParameter <bool> & parameter){
	ofxMidiMappableBool * mappable = new ofxMidiMappableBool(parameter, ++_idCounter);

	ofAddListener(mappable->getMapEvent(), this, &ofxMidiMapper::onMapEvent);
	_mappables[_idCounter] = mappable;
}
void ofxMidiMapper::addParameter(ofParameter <float> & parameter){
	ofxMidiMappableFloat * mappable = new ofxMidiMappableFloat(parameter, ++_idCounter);
//    mappable.addListener(*this);
	ofAddListener(mappable->getMapEvent(), this, &ofxMidiMapper::onMapEvent);
	_mappables[_idCounter] = mappable;
}
void ofxMidiMapper::addParameter(ofParameter <int> & parameter){
	ofxMidiMappableInt * mappable = new ofxMidiMappableInt(parameter, ++_idCounter);
//    mappable.addListener(*this);
	ofAddListener(mappable->getMapEvent(), this, &ofxMidiMapper::onMapEvent);
	_mappables[_idCounter] = mappable;
}
void ofxMidiMapper::addParameter(ofParameter <void> & parameter){
	ofxMidiMappableVoid * mappable = new ofxMidiMappableVoid(parameter, _idCounter);
//    mappable.addListener(*this);
	ofAddListener(mappable->getMapEvent(), this, &ofxMidiMapper::onMapEvent);
	_mappables[_idCounter] = mappable;
}
void ofxMidiMapper::addParameters(ofParameterGroup & parameters){
    for(auto parameter : parameters){
        auto type = parameter->type();
        ofLogNotice() << type;
        if(type == "11ofParameterIvE"){
            ofLogNotice("ofxMidiMapper") << "adding void parameter " << parameter->getName();
            auto typedParameter = parameter->cast<void>();
            addParameter(typedParameter);
        }else if(type == "11ofParameterIbE"){
            auto typedParameter = parameter->cast<bool>();
            ofLogNotice("ofxMidiMapper") << "adding bool parameter " << parameter->getName();
            addParameter(typedParameter);
        }else if(type == "11ofParameterIiE"){
            ofLogNotice("ofxMidiMapper") << "adding int parameter " << parameter->getName();
            auto typedParameter = parameter->cast<int>();
            addParameter(typedParameter);
        }else if(type == "11ofParameterIfE"){
            ofLogNotice("ofxMidiMapper") << "adding float parameter " << parameter->getName();
            auto typedParameter = parameter->cast<float>();
            addParameter(typedParameter);
        }
    }
}
void ofxMidiMapper::openMidiPort(int port){
	_midiIn.closePort();
	_midiIn.openPort(port);
}

void ofxMidiMapper::openVirtualMidiPort(string name){
	_midiIn.closePort();
	_midiIn.openVirtualPort(name);
}

bool ofxMidiMapper::addMapping(int channel, int pitch, bool isCC, int id, bool force){
	if(doesMappingExist(channel, pitch, isCC)){
		ofLogWarning("ofxMidiMapper") << "mapping does already exist: (" << channel << ", " << id << ")";
		if(force){
			_mapping[tuple < int, int, bool > (channel, pitch, isCC)] = id;
			return true;
		}else{
			return false;
		}
	}else{
		_mapping[std::tuple < int, int, bool > (channel, pitch, isCC)] = id;
		return true;
	}
}
bool ofxMidiMapper::removeMapping(int channel, int pitch, bool isCC){
	if(doesMappingExist(channel, pitch, isCC)){
		_mapping.erase(_mapping.find(std::tuple <int, int, bool>(channel, pitch, isCC)));
		return true;
	}else{
		ofLogWarning("ofxMidiMapper") << "mapping does not exist. are you sure you are doing right?";
		return false;
	}
}

bool ofxMidiMapper::doesMappingExist(int channel, int pitch, bool isCC){
	return (_mapping.find(std::tuple <int, int, bool>(channel, pitch, isCC)) != _mapping.end());
}

bool ofxMidiMapper::doesMappableExist(int id){
	return (_mappables.find(id) != _mappables.end());
}

int ofxMidiMapper::getMappedId(int channel, int pitch, bool isCC){
	if(doesMappingExist(channel, pitch, isCC)){
		return _mapping[std::tuple < int, int, bool > (channel, pitch, isCC)];
	}else{
//        TODO: better throw an exception instead of returning an empty string?
		ofLogError("ofxMidiMapper") << "mapping does not exist. returning empty string";
		return -1;
	}
}

ofxMidiMappable * ofxMidiMapper::getMappable(int id){
	if(doesMappableExist(id)){
		return _mappables[id];
	}else{
//        TODO: better throw an exception instead of returning nullptr?
		ofLogError("ofxMidiMapper") << "mappable does not exit. returning nullptr";
		return nullptr;
	}
}

ofParameterGroup & ofxMidiMapper::getParameters(){
	return _parameters;
}

void ofxMidiMapper::newMidiMessage(ofxMidiMessage & msg){
	if(!_activeParameter){
		return;
	}

	switch(msg.status){
	 case MIDI_NOTE_ON: {
		 int channel = msg.channel;
		 int pitch = msg.pitch;
		 int velocity = msg.velocity;
		 bool isCC = false;
		 if(_activeMappingParameter){
			 if(_idOfMappable != -1){
				 addMapping(channel, pitch, isCC, _idOfMappable);
			 }
			 _idOfMappable = -1;
		 }
		 if(doesMappingExist(channel, pitch, isCC)){
			 auto id = getMappedId(channel, pitch, isCC);
			 if(doesMappableExist(id)){
				 getMappable(id)->map(velocity);
			 }
		 }else{
//            ofLogNotice("ofxMidiMapper")<<"mapping does not already exist";
		 }
		 break;
	 }

	 case MIDI_NOTE_OFF: {
		 int channel = msg.channel;
		 int pitch = msg.pitch;
		 int velocity = msg.velocity;
		 bool isCC = false;
		 if(doesMappingExist(channel, pitch, isCC)){
			 auto id = getMappedId(channel, pitch, isCC);
		 }
		 break;
	 }

	 case MIDI_CONTROL_CHANGE: {
		 int channel = msg.channel;
		 int value = msg.value;
		 int control = msg.control;
		 bool isCC = true;
		 if(_activeMappingParameter){
			 if(_idOfMappable != -1){
				 addMapping(channel, control, isCC, _idOfMappable);
			 }
			 _idOfMappable = -1;
		 }
		 if(doesMappingExist(channel, control, isCC)){
			 auto id = getMappedId(channel, control, isCC);
			 if(doesMappableExist(id)){
				 getMappable(id)->map(value);
			 }
		 }else{
//            ofLogNotice("ofxMidiMapper")<<"mapping does not already exist";
		 }
		 break;
	 }

	 case MIDI_PROGRAM_CHANGE:
		 break;

	 default:
		 break;
	}
}

void ofxMidiMapper::onMapEvent(int & idOfMappable){
	if(_activeMappingParameter){
		_idOfMappable = idOfMappable;
	}
}

bool ofxMidiMapper::loadMapping(string path){
	ofFile mappingFile;
	ofJson mappingJson;
	mappingFile.open(ofToDataPath(path));
	if(mappingFile.exists()){
        mappingJson << mappingFile;
        ofLogNotice("ofxMidiMapper") << "successfully loaded mappings " << mappingJson.dump(4);
        _mapping.clear();

        for(auto mapping : mappingJson["mappings"]){
            addMapping(mapping["channel"].get<int>(), mapping["pitchOrCC"].get<int>(), mapping["isCC"].get<bool>(), mapping["id"].get<int>());
        }
	}else{
		ofLogError("ofxMidiMapper") << ofToDataPath(path) << " does not exist";
		return false;
	}
}

bool ofxMidiMapper::saveMapping(string path, bool force){
	ofFile mappingFile;
	mappingFile.open(ofToDataPath(path), ofFile::ReadWrite);
	if(mappingFile.exists() && !force){
		ofLogError("ofxMidiMapper") << ofToDataPath(path) << " already exists.";
		return false;
	}else{
		mappingFile.create();
	}
	ofJson mappingJson;

	mappingJson["mappings"] = ofJson::array();
	for(auto mapping : _mapping){
		int channel = std::get<0>(mapping.first);
		int pitchOrCC = std::get<1>(mapping.first);
		bool isCC = std::get<2>(mapping.first);

		int id = mapping.second;
        mappingJson["mappings"].push_back({{"channel", channel}, {"pitchOrCC", pitchOrCC}, {"isCC", isCC}, {"id", id}});
	}
	mappingFile << mappingJson.dump(4);
	mappingFile.close();
	return true;
}
