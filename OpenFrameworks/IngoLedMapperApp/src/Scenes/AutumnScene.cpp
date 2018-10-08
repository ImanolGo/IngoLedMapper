/*
 *  AutumnScene.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 07/10/18.
 *
 */



#include "AutumnScene.h"
#include "AppManager.h"


AutumnScene::AutumnScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

AutumnScene::~AutumnScene()
{
    //Intentionally left empty
}


void AutumnScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setuShader();
}

void AutumnScene::setuShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/Sunrise.frag");
    }
    else{
        m_shader.load("shaders/shadersGL2/Sunrise");
    }
    
}


void AutumnScene::update()
{
    //Empty
}

void AutumnScene::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawShader();
    ofDisableAlphaBlending();
   
}


void AutumnScene::drawShader()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto parameters = AppManager::getInstance().getParticlesManager().getParameters();
    
    
        if(ofIsGLProgrammableRenderer())
        {
            m_shadertoy.begin();
            m_shadertoy.setUniform3f("iResolution", width, height, 0.0);
            m_shadertoy.setUniform1f("iTime", ofGetElapsedTimef()*parameters.speed);
                ofDrawRectangle(0, 0, width, height);
            m_shadertoy.end();
        }
    
     else{
         
         m_shader.begin();
         m_shader.setUniform3f("iResolution", width, height, 0.0);
         m_shader.setUniform1f("iTime", ofGetElapsedTimef()*parameters.speed);
         ofDrawRectangle(0, 0, width, height);
         m_shader.end();
     }
    
    
   // m_gradient.drawDebug(0,0, width, height);
}


void AutumnScene::willFadeIn() {
    AppManager::getInstance().getGuiManager().loadPresetsValues(getName());
    ofLogNotice("AutumnScene::willFadeIn");
}

void AutumnScene::willDraw() {
    ofLogNotice("AutumnScene::willDraw");
}

void AutumnScene::willFadeOut() {
    AppManager::getInstance().getGuiManager().savePresetsValues(getName());
    ofLogNotice("AutumnScene::willFadeOut");
}

void AutumnScene::willExit() {
     ofLogNotice("AutumnScene::willExit");
}

