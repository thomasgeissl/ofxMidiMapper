#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableFloat : public ofxMidiMappable {
	public:
		void setup(ofParameter <float> * parameter){
			_parameter = parameter;
			parameter->addListener(this, &ofxMidiMappableFloat::notify);
		}
		void map(int value){
			_parameter->set(ofMap(value, 0, 127, _parameter->getMin(), _parameter->getMax()));
		}

	protected:
		void notify(float & value){
			string name = _parameter->getName();
			_mapEvent.notify(name);
		}
	protected:
		ofParameter <float> * _parameter;

	private:
};
