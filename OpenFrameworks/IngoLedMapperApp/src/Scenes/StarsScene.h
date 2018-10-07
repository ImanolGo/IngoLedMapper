/*
 *  StarsScene.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 07/10/18.
 *
 */



#pragma once

#include "ofxScene.h"

class StarsScene : public ofxScene {
    
public:
    
    //! Constructor
    StarsScene(std::string name);
    
    //! Destructor
    ~StarsScene();
    
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

};




