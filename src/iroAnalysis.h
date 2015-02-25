#pragma once
#include "ofMain.h"

class iroAnalysis: public ofThread {
private:
    void threadedFunction();
    ofThreadChannel<ofPixels> toAnalize;
    ofThreadChannel<bool> analized;
    vector<ofColor>analizedColor;
    int scale;
    int width;
    int size;
    bool isAnalaized;
    bool newFrame;
    
public:
    iroAnalysis();
    ~iroAnalysis();
    
    void setScale(int num) { scale = num; }
    void setSize(int s) { size = s; }
    void analize(ofPixels & pixels);
    void update();
    void draw(float x, float y);
    
    bool isFrameNew() { return newFrame; }
    vector<ofColor>getColor() { return analizedColor; }
};
