//
//  iroAnalysis.h
//
//  Created by azuremous on 2/23/15.

#include "iroAnalysis.h"

//--------------------------------------------------------------
iroAnalysis::iroAnalysis()
:newFrame(true),
scale(20),
size(4),
width(0)
{
	startThread();
}

//--------------------------------------------------------------
iroAnalysis::~iroAnalysis(){

	toAnalize.close();
	analized.close();
	waitForThread(true);
}

//--------------------------------------------------------------
void iroAnalysis::analize(ofPixels & pixels){
	toAnalize.send(pixels);
}

//--------------------------------------------------------------
void iroAnalysis::update(){
	newFrame = false;
	while(analized.tryReceive(isAnalaized)){
		newFrame = true;
	}
}

//--------------------------------------------------------------
void iroAnalysis::draw(float x, float y){
    if (analizedColor.size() > 0) {
        ofPushMatrix();
        ofTranslate(x, y);
        ofPushStyle();
        for (int i = 0; i < analizedColor.size(); i++) {
            ofSetColor(analizedColor[i],255);
            ofDrawRectangle((i% width)*size, (i / width)*size, size, size);
        }
        ofPopStyle();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void iroAnalysis::threadedFunction(){
	while(isThreadRunning()){
		ofPixels pixels;
		if(toAnalize.receive(pixels)){
            vector<ofColor>pixelColors;
            width = pixels.getWidth() / scale;
            for (int y = 0; y < pixels.getHeight() - scale; y+=scale) {
                for (int x = 0; x < pixels.getWidth() - scale; x+=scale) {
                    float r,g,b,a;
                    a = pixels.getColor(x, y).a;
                    if (a > 0) {
                        ofColor c = pixels.getColor(x, y);
                        pixelColors.push_back(c);
                    }
                }
            }
            analizedColor = pixelColors;
            analized.send(true);
        }else{
			break;
		}
	}
}

