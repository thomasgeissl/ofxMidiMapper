#include "ofxMidiMappable.h"

ofxMidiMappable::ofxMidiMappable(int id) : _id(id){}

void ofxMidiMappable::addListener(ofxMidiMapper & listener){
//    ofAddListener(_mapEvent, &listener, &ofxMidiMapper::onMapEvent);
}

ofEvent <int> & ofxMidiMappable::getMapEvent(){
	return _mapEvent;
}
