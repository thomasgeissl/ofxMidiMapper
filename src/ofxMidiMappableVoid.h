#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableVoid : public ofxMidiMappable {
	public:
		void setup(ofParameter <void> * parameter){
			_parameter = parameter;
			parameter->addListener(this, &ofxMidiMappableVoid::notify);
		}
		void map(int value){
//			_parameter->set(ofMap(value, 0, 127, _parameter->getMin(), _parameter->getMax()));
		}

	protected:
		void notify(){
			string name = _parameter->getName();
			_mapEvent.notify(name);
		}
	protected:
		ofParameter <void> * _parameter;

	private:
};
