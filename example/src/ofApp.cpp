#include "ofApp.h"



void ofApp::setup(){
	ofSetFrameRate(120);
    _parameters.setName("parameters");
    _mappableParameters.setName("mappable parameters");
    _mappableBoolParameter.set("bool parameter", false);
    _mappableFloatParameter.set("float parameter", 0,0,1);
    _mappableIntParameter.set("int parameter", 0,0,100);

    _mappableParameters.add(_mappableBoolParameter);
    _mappableParameters.add(_mappableFloatParameter);
    _mappableParameters.add(_mappableIntParameter);

    _midiMapper.getParameters().setName("Mapper parameters");
    _parameters.add(_midiMapper.getParameters());
    _parameters.add(_mappableParameters);

    _gui.setup(_parameters);

    _midiMapper.addParameter(&_mappableBoolParameter);
    _midiMapper.addParameter(&_mappableFloatParameter);
    _midiMapper.addParameter(&_mappableIntParameter);
}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    _gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
