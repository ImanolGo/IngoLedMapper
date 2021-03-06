/*
 *  NorthernLightsScene.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 21/09/18.
 *
 */


#include "NorthernLightsScene.h"
#include "AppManager.h"


NorthernLightsScene::NorthernLightsScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

NorthernLightsScene::~NorthernLightsScene()
{
    //Intentionally left empty
}


void NorthernLightsScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupShader();
}

void NorthernLightsScene::setupShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/NorthernLights.frag");
    }
    else{
        m_shader.load("shaders/shadersGL2/NorthernLights");
    }
    
}



void NorthernLightsScene::update()
{
    //Empty
}

void NorthernLightsScene::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawShader();
    ofDisableAlphaBlending();
   
}


void NorthernLightsScene::drawShader()
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
        //m_shader.setUniform3f("iColor",color.r/255.0,color.g/255.0,color.b/255.0);
        //m_shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0.0);
        m_shader.setUniform3f("iResolution", width, height, 0.0);
        m_shader.setUniform1f("iTime", ofGetElapsedTimef()*parameters.speed);
            ofDrawRectangle(0, 0, width, height);
        m_shader.end();
     }
}


void NorthernLightsScene::willFadeIn() {
    AppManager::getInstance().getGuiManager().loadPresetsValues(getName());
    ofLogNotice("NorthernLightsScene::willFadeIn");    
}

void NorthernLightsScene::willDraw() {
    ofLogNotice("NorthernLightsScene::willDraw");
}

void NorthernLightsScene::willFadeOut() {
    AppManager::getInstance().getGuiManager().savePresetsValues(getName());
    ofLogNotice("NorthernLightsScene::willFadeOut");
}

void NorthernLightsScene::willExit() {
     ofLogNotice("NorthernLightsScene::willExit");
}

