/*
 *  NorthernLightsScene2.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 21/09/18.
 *
 */



#pragma once

#include "ofxScene.h"

class NorthernLightsScene2 : public ofxScene {
    
public:
    
    //! Constructor
    NorthernLightsScene2(std::string name);
    
    //! Destructor
    ~NorthernLightsScene2();
    
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
    
    void setupShader();
    
    void drawShader();
    
private:
    
    ofColor         m_color;
    ofShader        m_shader;
    ofTexture       m_channel1;
    ofTexture       m_channel2;

};




