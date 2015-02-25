#include "ofApp.h"

vector<ofColor>pixelColors;
vector<vector<ofColor> >cieColor;

string deltaEstring[] = { "delta", "filter", "result" };
float white = 248.;
float difference = 32.;
bool getColor = false;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    grabber.setDeviceID(0);
    grabber.setup(512,424);
    
    cieColor.resize(3);//3
    iroChecker.setup(49., 247);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    grabber.update();
    if(grabber.isFrameNew()){
        analizer.analize(grabber.getPixels());
        analizer.update();
        if (analizer.isFrameNew()) {
            pixelColors = analizer.getColor();
            if(getColor){
                iroChecker.analize(pixelColors);
                getColor = false;
            }
        }
        iroChecker.update();
        if (iroChecker.isFrameNew()) {
            cieColor = iroChecker.getColors();
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
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
    
    analizer.draw(200, 200);
    grabber.draw(640,0);
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
