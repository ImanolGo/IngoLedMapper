/*
 *  NoiseScene.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#pragma once

#include "ofxScene.h"
#include "ofxColorGradient.h"

class NoiseScene : public ofxScene {
    
public:
    
    //! Constructor
    NoiseScene(std::string name);
    
    //! Destructor
    ~NoiseScene();
    
    //! Set up the scene
    void setup();
    
    //! Update the scene
    void update();
    
    //! Draw the scene
    void draw();
    
    //! Called when fading in
    void willFadeIn();
    
    //! Called when to start drawing
    void willDraw();
    
    //! Called fading out
    void willFadeOut();
    
    //! Called when exit
    void willExit();
    
private:
    
    void setupNoiseShader();
    
    void setupGradient();
    
    void drawNoise();
    
private:
    
    ofColor                     m_color;
    ofShader                    m_noiseShader;
    ofxColorGradient<ofColor>   m_gradient;

};




