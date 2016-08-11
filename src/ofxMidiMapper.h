#pragma once

#include <tuple>

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMidiMappable.h"
#include "ofxMidiMappableBool.h"
#include "ofxMidiMappableFloat.h"
#include "ofxMidiMappableInt.h"
#include "ofxMidiMappableVoid.h"



class ofxMidiMapper :
	public ofxMidiListener {
	public:
		ofxMidiMapper();
		void addParameter(ofParameter <bool> * parameter);
		void addParameter(ofParameter <float> * parameter);
		void addParameter(ofParameter <int> * parameter);
		void addParameter(ofParameter <void> * parameter);

		void openMidiPort(int port);
		void openVirtualMidiPort(string name);

		bool addMapping(int channel, int pitch, bool isCC, std::string name, bool force = true);
		bool removeMapping(int channel, int pitch, bool isCC);

		bool doesMappingExist(int channel, int pitch, bool isCC);
		bool doesMappableExist(std::string name);
		std::string getMappedName(int channel, int pitch, bool isCC);
		ofxMidiMappable * getMappable(std::string name);
		ofParameterGroup & getParameters();
		void onMapEvent(string & nameOfMappable);

		bool loadMapping(std::string path);
		bool saveMapping(std::string path, bool force = true);

	protected:
	private:
		void newMidiMessage(ofxMidiMessage & msg);

	private:
		ofxMidiIn _midiIn;
		std::map <std::string, ofxMidiMappable *> _mappables;

//    std::map<std::pair<int, int>, string> _mapping;
		std::map <std::tuple <int, int, bool>, string> _mapping;

		ofParameterGroup _parameters;
		ofParameter <bool> _activeParameter;
		ofParameter <bool> _activeMappingParameter;

		std::string _nameOfMappable;
};
