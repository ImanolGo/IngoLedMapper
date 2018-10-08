/*
 *  StarsScene.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 07/10/18.
 *
 */


#include "StarsScene.h"
#include "AppManager.h"


StarsScene::StarsScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

StarsScene::~StarsScene()
{
    //Intentionally left empty
}


void StarsScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupShader();
}

void StarsScene::setupShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/Stars2.frag");
        m_shadertoy.setAdvanceTime(true);
    }
    else{
        m_shader.load("shaders/shadersGL2/Stars2");
    }
}



void StarsScene::update()
{
    //Empty
}

void StarsScene::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawShader();
    ofDisableAlphaBlending();
   
}


void StarsScene::drawShader()
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
        
        //m_shadertoy.draw(0, 0, width, height);
    }
    
    else{
        
        m_shader.begin();
        //m_shader.setUniform3f("iColor",color.r/255.0,color.g/255.0,color.b/255.0);
        //m_shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0.0);
        m_shader.setUniform3f("iResolution", width, height, 0.0);
        m_shader.setUniform1f("iTime", ofGetElapsedTimef()*parameters.speed);
        ofDrawRectangle(0, 0, width, height);
        m_shader.end();
    }
}


void StarsScene::willFadeIn() {
    AppManager::getInstance().getGuiManager().loadPresetsValues(getName());
    ofLogNotice("StarsScene::willFadeIn");    
}

void StarsScene::willDraw() {
    ofLogNotice("StarsScene::willDraw");
}

void StarsScene::willFadeOut() {
    AppManager::getInstance().getGuiManager().savePresetsValues(getName());
    ofLogNotice("StarsScene::willFadeOut");
}

void StarsScene::willExit() {
     ofLogNotice("StarsScene::willExit");
}

