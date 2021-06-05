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
	struct midiInPort {
		bool _active;
		std::shared_ptr<ofxMidiIn> _midiIn;
	};
		ofxMidiMapper();
		void update(){
			scanMidiPorts();
		}
		void addParameter(ofParameter <bool> & parameter);
		void addParameter(ofParameter <float> & parameter, bool catchCurrentValue = false);
		void addParameter(ofParameter <int> & parameter, bool catchCurrentValue = false);
		void addParameter(ofParameter <void> & parameter);
        void addParameters(ofParameterGroup & parameters, bool catchCurrentValue = false);

		void openMidiPort(int port);
		void openVirtualMidiPort(string name);
        void listInPorts();

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

		ofParameter<bool> & getActiveParameter() {
			return _activeParameter;
		}
		bool isActive() {
			return _activeParameter;
		}
		ofParameter<bool> & getActiveMappingParameter() {
			return _activeMappingParameter;
		}
		void setActiveMapping(bool value)
		{
			_activeMappingParameter = value;
		}

		void clear()
		{
			_mapping.clear();
		}
		void scanMidiPorts()
		{
			auto timestamp = ofGetElapsedTimeMillis();
			if(timestamp - _scanTimestamp < 5000) return;
			ofLogVerbose() << "detecting new midi ports ...";
			for(auto port : _midiIn.getInPortList()){
				auto newPort = true;
				for(auto portToCompareWith : _midiPorts){
					if(port == portToCompareWith._midiIn->getName()){
						newPort = false;
					}
				}
				if(newPort){
					ofLogVerbose() << "found new midi port"; 
					midiInPort p;
					p._active = true;
					p._midiIn = std::make_shared<ofxMidiIn>();
					_midiPorts.push_back(p);
					_midiPorts.back()._midiIn->openPort(port);
					_midiPorts.back()._midiIn->addListener(this);
				}
			}
			_scanTimestamp = timestamp;
		}
		std::vector<midiInPort> getPorts(){
			return _midiPorts;
		}
		std::map<std::tuple<int, int, bool>, int> getMappings(){
			return _mapping;
		};
		void setMidiPortActive(string name, bool value)
		{
			for(auto & port: _midiPorts){
				if(port._active != value && port._midiIn->getName() == name){
					port._active = value;
					if(value){
						ofLogNotice() << "adding new listener";
						port._midiIn->addListener(this);
					}else{
						ofLogNotice() << "removing listener";
						port._midiIn->removeListener(this);
					}
				}
			}
		}

	protected:
	private:
		void newMidiMessage(ofxMidiMessage & msg);

	private:
		ofxMidiIn _midiIn;
		// std::vector<std::shared_ptr<ofxMidiIn>> _midiPorts;
		std::vector<midiInPort> _midiPorts;
		std::map<int, ofxMidiMappable*> _mappables;
		std::map<std::tuple<int, int, bool>, int> _mapping;
        std::vector<ofParameter<bool>> _params;

		ofParameterGroup _parameters;
		ofParameter <bool> _activeParameter;
		ofParameter <bool> _activeMappingParameter;

		int _idOfMappable;
        int _idCounter;
		uint64_t _scanTimestamp;
};
