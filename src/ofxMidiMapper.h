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
        void addParameters(ofParameterGroup & parameters);

		void openMidiPort(int port);
		void openVirtualMidiPort(string name);

		bool addMapping(int channel, int pitch, bool isCC, int id, bool force = true);
		bool removeMapping(int channel, int pitch, bool isCC);

		bool doesMappingExist(int channel, int pitch, bool isCC);
		bool doesMappableExist(int id);
		int getMappedId(int channel, int pitch, bool isCC);
		ofxMidiMappable * getMappable(int id);
		ofParameterGroup & getParameters();
		void onMapEvent(int & idOfMappable);

		bool loadMapping(std::string path);
		bool saveMapping(std::string path, bool force = true);

	protected:
	private:
		void newMidiMessage(ofxMidiMessage & msg);

	private:
		ofxMidiIn _midiIn;
		std::map<int, ofxMidiMappable *> _mappables;
		std::map<std::tuple<int, int, bool>, int> _mapping;

		ofParameterGroup _parameters;
		ofParameter <bool> _activeParameter;
		ofParameter <bool> _activeMappingParameter;

		int _idOfMappable;
        int _idCounter;
};
