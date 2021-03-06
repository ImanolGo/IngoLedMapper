/*
 *  CloudScene.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#pragma once

#include "ofxScene.h"
#include "ofxShadertoy.h"

class CloudScene : public ofxScene {
    
public:
    
    //! Constructor
    CloudScene(std::string name);
    
    //! Destructor
    ~CloudScene();
    
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
    
    void setupFbo();
    
    void setupShader();
    
    void drawShader();
    
private:
    
    ofColor         m_color;
    ofFbo           m_fbo;
    ofShader        m_shader;
    ofxShadertoy    m_shadertoy;


};




