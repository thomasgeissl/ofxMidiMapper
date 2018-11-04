#include "ofxMidiMappable.h"

void ofxMidiMappable::addListener(ofxMidiMapper & listener){
//    ofAddListener(_mapEvent, &listener, &ofxMidiMapper::onMapEvent);
}

ofEvent <std::string> & ofxMidiMappable::getMapEvent(){
	return _mapEvent;
}
