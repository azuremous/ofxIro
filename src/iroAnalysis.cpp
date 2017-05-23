//
//  iroAnalysis.h
//
//  Created by azuremous on 2/23/15.

#include "iroAnalysis.h"

//--------------------------------------------------------------
iroAnalysis::iroAnalysis()
:isAnalaized(true),
scale(20),
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
void iroAnalysis::update(ofPixels & pixels){
	toAnalize.send(pixels);
    if(!analized.tryReceive(isAnalaized)) isAnalaized = false;
}

//--------------------------------------------------------------
void iroAnalysis::draw(float x, float y){ //for debug
    if (isFrameNew()) {
        ofPushMatrix();
        ofTranslate(x, y);
        ofPushStyle();
        for (int i = 0; i < analizedColor.size(); i++) {
            ofSetColor(analizedColor[i],255);
            ofDrawRectangle((i% width)*scale, (i/width)*scale, scale, scale);
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
            int height = pixels.getHeight() / scale;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float rx = x*scale + scale/2;
                    float ry = y*scale + scale/2;
                    float a = pixels.getColor(rx, ry).a;
                    if (a > 0.) {
                        ofColor c = pixels.getColor(rx, ry);
                        pixelColors.push_back(c);
                    }
                }
            }
            analizedColor = pixelColors;
            if(analizedColor.size() > 0) { analized.send(true); }
        }else{
			break;
		}
	}
}

