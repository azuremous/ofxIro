#pragma once
#include "ofMain.h"

class iroAnalysis: public ofThread {
private:
    
    ofThreadChannel<ofPixels> toAnalize;
    ofThreadChannel<bool> analized;
    vector<ofColor>analizedColor;
    int scale;
    int width;
    bool isAnalaized;
    
protected:
    void threadedFunction();
    
public:
    iroAnalysis();
    ~iroAnalysis();
    
    void setup(int scale) { this->scale = scale; }
    void update(ofPixels & pixels);
    void draw(float x, float y);
    bool isFrameNew() { return isAnalaized; }
    vector<ofColor>getColor() { return analizedColor; }
};
