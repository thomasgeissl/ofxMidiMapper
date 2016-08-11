#pragma once
#include "ofxMidiMappable.h"

class ofxMidiMappableInt : public ofxMidiMappable {
	public:
		void setup(ofParameter <int> * parameter){
			_parameter = parameter;
			parameter->addListener(this, &ofxMidiMappableInt::notify);
		}
		void map(int value){
			_parameter->set(ofMap(value, 0, 127, _parameter->getMin(), _parameter->getMax()));
		}

	protected:
		void notify(int & value){
			string name = _parameter->getName();
			_mapEvent.notify(name);
		}
	protected:
		ofParameter <int> * _parameter;

	private:
};
