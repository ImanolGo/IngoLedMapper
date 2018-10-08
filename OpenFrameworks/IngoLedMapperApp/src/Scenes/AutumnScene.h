/*
 *  AutumnScene.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 07/10/18.
 *
 */



#pragma once

#include "ofxScene.h"
#include "ofxShadertoy.h"

class AutumnScene : public ofxScene {
    
public:
    
    //! Constructor
    AutumnScene(std::string name);
    
    //! Destructor
    ~AutumnScene();
    
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
    
    void setuShader();
    
    void drawShader();
    
private:
    
    ofColor                     m_color;
    ofShader                    m_shader;
    ofxShadertoy                m_shadertoy;

};




