#pragma once
#include "ofParameter.h"

class ofxMidiMapper;
class ofxMidiMappable {
	public:
		void addListener(ofxMidiMapper & listener);
		virtual void map(int value){
		}
		ofEvent <string> & getMapEvent();

	protected:
		ofEvent <string> _mapEvent;

	private:
};
