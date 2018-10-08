/*
 *  ColorGradientScene.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#pragma once

#include "ofxScene.h"
#include "ofxColorGradient.h"
#include "ofxFboBlur.h"
#include "ofxShadertoy.h"


class ColorGradientScene : public ofxScene {
    
public:
    
    //! Constructor
    ColorGradientScene(std::string name);
    
    //! Destructor
    ~ColorGradientScene();
    
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
    
    void setupFbo();
    
    void setupGradient();
    
    void setupBlur();
    
    void updateBlur();
    
    void drawGradient();
    
    void drawShader();
    
private:
    
    ofColor                     m_color;
    ofShader                    m_shader;
    ofxColorGradient<ofColor>   m_gradient;
    ofFbo            m_fbo;
    ofxFboBlur       m_blur;
    ofxShadertoy     m_shadertoy;

};




