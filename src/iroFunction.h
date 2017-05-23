//
//  iroFunction.h
//
//  Created by azuremous on 1/26/15.
//  of 0.9.0 +
//  color Comparisons
//
// reference : https://github.com/THEjoezack/ColorMine
//             http://en.wikipedia.org/wiki/Color_difference
//

#pragma once

#include "ofMain.h"

typedef enum {
    CIE76,
    CIE94,
    CIE2000
}DELTA_TYPE;

typedef enum {
    RAW,
    FILTER,
    NOWHITE,
}IRO_TYPE;

class iroFunction : public ofThread{
    
protected:
    double pivotRgb(double n){ return (n > 0.04045 ? pow((n + 0.055) / 1.055, 2.4) : n / 12.92) * 100.0; }
    double PivotXyz(double n) { return (n > 0.008856 ? pow(n, 0.333333) : (7.787 * n) + (16/116.0)); }
    
    bool compareColor76(const ofColor &c1, const ofColor &c2, float distance){
        
        ofVec3f col1, col2;
        
        col1 = convertXYZtoLAB(convertToXYZ(c1));
        col2 = convertXYZtoLAB(convertToXYZ(c2));
        
        double l = pow((col1.x - col2.x), 2);
        double a = pow((col1.y - col2.y), 2);
        double b = pow((col1.z - col2.z), 2);
        double result = sqrt (l+a+b);
        return distance >= result;
    }
    
    bool compareColor94(const ofColor &c1, const ofColor &c2, float distance,  int type = 0){
        
        ofVec3f col1, col2;
        
        col1 = convertXYZtoLAB(convertToXYZ(c1));
        col2 = convertXYZtoLAB(convertToXYZ(c2));
        
        //l,a,b
        double C1 = sqrt(col1.y * col1.y + col1.z * col1.z);
        double C2 = sqrt(col2.y * col2.y + col2.z * col2.z);
        double delta_L = col1.x - col2.x;
        double delta_C = C1 - C2;
        double delta_a = col1.y - col2.y;
        double delta_b = col1.z - col2.z;
        double delta_H = sqrt(delta_a * delta_a + delta_b * delta_b - delta_C * delta_C);
        
        if (type == 0) {
            delta_C /= C1 * 0.045 + 1;
            delta_H /= C1 * 0.015 + 1;
        }else if (type == 1) {
            delta_L *= 0.5;
            delta_C /= C1 * 0.048 + 1;
            delta_H /= C1 * 0.014 + 1;
        }
        
        double result = sqrt(delta_L * delta_L + delta_C * delta_C + delta_H * delta_H);
        
        return distance >= result;
        
    }
    
    bool compareColor2k(const ofColor &c1, const ofColor &c2, float distance){
        
        ofVec3f col1, col2;
        
        col1 = convertLABtoLCH(convertXYZtoLAB(convertToXYZ(c1)));
        col2 = convertLABtoLCH(convertXYZtoLAB(convertToXYZ(c2)));
        
        double avg_L = (col1.x + col2.x) * 0.5;
        double delta_L = col2.x - col1.x;
        
        double avg_C = (col1.y + col2.y) * 0.5;
        double delta_C = col1.y - col2.y;
        
        double avg_H = (col1.z + col2.z) * 0.5;
        
        if (abs(col1.z - col2.z) > PI) { avg_H += PI; }
        
        double delta_H = col2.z - col1.z;
        
        if (abs(delta_H) > PI) {
            if (col2.z <= col1.z) { delta_H += PI * 2; }
            else { delta_H -= PI * 2; }
        }
        
        delta_H = sqrt(col1.y * col2.y) * sin(delta_H) * 2;
        
        double T = 1 - 0.17 * cos(avg_H - PI / 6.) + 0.24 * cos(avg_H * 2) + 0.32 * cos(avg_H * 3 + PI / 30.) - 0.20 * cos(avg_H * 4 - PI * 7./20.);
        
        double SL = avg_L - 50.;
        SL *= SL;
        SL = SL * 0.015 / sqrt(SL + 20) + 1;
        
        double SC = avg_C * 0.045 + 1;
        
        double SH = avg_C * T * 0.015 + 1;
        
        double delta_Theta = avg_H / 25. - PI * 11./180.;
        delta_Theta = exp(delta_Theta * -delta_Theta) * (PI / 6.);
        
        double RT = pow(avg_C, 7);
        RT = sqrt(RT / (RT + 6103515625)) * sin(delta_Theta) * -2.; // 6103515625 = 25^7
        
        delta_L /= SL;
        delta_C /= SC;
        delta_H /= SH;
        
        double result = sqrt(delta_L * delta_L + delta_C * delta_C + delta_H * delta_H + RT * delta_C * delta_H);
        
        return distance >= result;
    }
    
    vector<ofColor> makeGroup(DELTA_TYPE deltaType, const vector<ofColor>&c, float distance = 28.0){
        vector<ofColor>colorGroup;
        for (int i = 0; i < c.size(); i++) {
            ofColor lastColor;
            lastColor = c[i];
            if (i == 0) {
                colorGroup.push_back(lastColor);
            }else{
                int num = 0;
                
                for (int j = 0; j < colorGroup.size(); j++) {
                    if (deltaType == CIE76) { //CIE76
                        if (!compareColor76(colorGroup[j], lastColor, distance)) {//diff color
                            num++;
                        }
                    }else if(deltaType == CIE94){ //CIE94
                        if (!compareColor94(colorGroup[j], lastColor, distance)) {//diff color
                            num++;
                        }
                    }else if(deltaType == CIE2000){ //CIE2000
                        if (!compareColor2k(colorGroup[j], lastColor, distance)) {//diff color
                            num++;
                        }
                    }
                }
                if (num == colorGroup.size()) { //there is no group. make new group
                    colorGroup.push_back(lastColor);
                }
            }
        }
        return colorGroup;
    }
    
public:
    
    explicit iroFunction():
    difference(18.),
    white(248.),
    satBalance(0.4),
    briBalance(0.4),
    isUpdated(true)
    {
        startThread();
        deltaColor.resize(3);
    }
    
    ~iroFunction(){
        toAnalize.close();
        analized.close();
        waitForThread(true);
    }
    
    void setup(float diff, float w){
        setDifference(diff);
        setWhite(w);
    }
    
    void setDifference(float diff) { difference = diff; }
    void setWhite(float w) { white = w; }
    void setSatBalance(float s) { satBalance = s; }
    void setBriBalance(float b) { briBalance = b; }
    void setBalance(float s, float b) {
        setSatBalance(s);
        setBriBalance(b);
    }
    
    void update(vector<ofColor> & col, DELTA_TYPE type = CIE76){
        deltaType = type;
        toAnalize.send(col);
        if(!analized.tryReceive(isUpdated)) isUpdated = false;
    }
    
    bool isFrameNew(){ return isUpdated; }
    
    vector<ofColor>getColor(IRO_TYPE type) { return deltaColor[type]; }
    vector<vector<ofColor> >getColors() { return deltaColor; }
    
    ofVec3f convertToXYZ(const ofColor & c) {
        double r = pivotRgb(c.r/255.0);
        double g = pivotRgb(c.g/255.0);
        double b = pivotRgb(c.b/255.0);
        
        ofVec3f colXYZ;
        double x,y,z;
        x = r * 0.4124 + g * 0.3576 + b * 0.1805;
        y = r * 0.2126 + g * 0.7152 + b * 0.0722;
        z = r * 0.0193 + g * 0.1192 + b * 0.9505;
        
        colXYZ.set(x, y, z);
        
        return colXYZ;
    }
    
    ofVec3f convertXYZtoLAB(const ofVec3f &col){
        double _x = PivotXyz(col.x / 95.047);
        double _y = PivotXyz(col.y / 100.000);
        double _z = PivotXyz(col.z / 108.883);
        
        ofVec3f colLAB;
        float l,a,b;
        l = ( 116 * _y ) - 16;
        a = 500 * ( _x - _y );
        b = 200 * ( _y - _z );
        colLAB.set(l, a, b);
        
        return colLAB;
    }
    
    ofVec3f convertLABtoLCH(const ofVec3f & c){
        
        double h = atan2(c.z , c.y);
        if (h > 0) { h = (h / PI) * 180.; }
        else{ 360 - (abs(h) / PI) * 180.0; }
        
        if (h < 0){ h += 360.0; }
        else if (h >= 360) { h -= 360.0; }
        
        ofVec3f col;
        double c_ = sqrt(c.y* c.y + c.z * c.z);
        col.set(c.x, c_, h);
        
        return col;
    }
    
private:
    float difference;
    float white;
    float satBalance;
    float briBalance;
    DELTA_TYPE deltaType;
    bool isUpdated;
    vector<vector<ofColor> >deltaColor;
    ofThreadChannel<vector<ofColor> > toAnalize;
    ofThreadChannel<bool> analized;
    
    void threadedFunction(){
        while(isThreadRunning()){
            vector<ofColor>c;
            if (toAnalize.receive(c)) {
                for (int i = 0; i < deltaColor.size(); i++) { deltaColor[i].clear(); }
                //delta
                deltaColor[0] = makeGroup(deltaType, c, difference);
                //filter
                deltaColor[1] = deltaColor[0];
                for (int i = 0; i < deltaColor[1].size(); i++) {
                    deltaColor[1][i].setHue(deltaColor[0][i].getHue());
                    float sat = deltaColor[0][i].getSaturation() + deltaColor[0][i].getSaturation() * satBalance;
                    if (sat >= 255) sat = 255;
                    deltaColor[1][i].setSaturation(sat);
                    float bri = deltaColor[0][i].getBrightness() + deltaColor[0][i].getBrightness() * briBalance;
                    if (bri >= 255) bri = 255;
                    deltaColor[1][i].setBrightness(bri);
                }
                //result
                deltaColor[2] = makeGroup(deltaType, deltaColor[1], 38.);
                //delete white
                for (int i = 0; i < deltaColor[2].size(); i++) {
                    if (deltaColor[2][i].r >= white && deltaColor[2][i].g >= white && deltaColor[2][i].b >= white) {
                        deltaColor[2].erase(deltaColor[2].begin() + i);
                    }
                }
                if(deltaColor[0].size() > 0) { analized.send(true); }
            }else{
                break;
            }
        }
        
    }
};
