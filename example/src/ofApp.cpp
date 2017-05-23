#include "ofApp.h"

vector<ofColor>pixelColors;
vector<vector<ofColor> >cieColor;

string deltaEstring[] = { "delta", "filter", "result" };
float white = 248.;
float difference = 22.;
bool getColor = false;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    grabber.setDeviceID(0);
    grabber.setup(512,424);
    
    cieColor.resize(3);//3
    iroChecker.setup(difference, white);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    grabber.update();
    if(grabber.isFrameNew()){
        analyzer.update(grabber.getPixels());
        if (analyzer.isFrameNew()) {
            vector<ofColor>p = analyzer.getColor();
            iroChecker.update(p, CIE94);
            if (iroChecker.isFrameNew()) {
                cieColor = iroChecker.getColors();
            }
            if (iroChecker.isFrameNew() && getColor) {
                pixelColors = iroChecker.getColor(NOWHITE);
                getColor = false;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    analyzer.draw(20, 20);
    if (cieColor[0].size() > 0) {
        ofPushMatrix();
        ofPushStyle();
        ofDisableAlphaBlending();
        for (int i = 0; i < cieColor.size(); i++) {
            
            ofDrawBitmapStringHighlight(deltaEstring[i], 10, 20 + 50 *i);
            for (int j = 0; j < cieColor[i].size(); j++) {
                ofSetColor(cieColor[i][j]);
                int pixSize = 20;
                int rawSize = 100;
                ofDrawRectangle(10+(j% rawSize)*pixSize, 30*(i+1)+ 20*i + (j / rawSize)*pixSize, pixSize, pixSize);
            }
        }
        ofPopStyle();
        ofPopMatrix();
    }
    
    if(pixelColors.size() > 0){
        for(int i = 0; i < pixelColors.size(); i++){
            ofSetColor(pixelColors[i]);
            ofDrawRectangle(20 + 40*i, 500, 40, 40);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        getColor = true;
    }
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
