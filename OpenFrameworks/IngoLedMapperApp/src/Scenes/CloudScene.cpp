/*
 *  CloudScene.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#include "CloudScene.h"
#include "AppManager.h"


CloudScene::CloudScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

CloudScene::~CloudScene()
{
    //Intentionally left empty
}


void CloudScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupFbo();
    this->setupShader();
}

void CloudScene::setupShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/Clouds.frag");
    }
    else{
        m_shader.load("shaders/shadersGL2/Clouds");
    }
}


void CloudScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void CloudScene::update()
{
    //Empty
}

void CloudScene::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawShader();
    ofDisableAlphaBlending();
   
}


void CloudScene::drawShader()
{
    float width = m_fbo.getWidth();
    float  height =  m_fbo.getHeight();
    
    auto parameters = AppManager::getInstance().getParticlesManager().getParameters();
    
    float cloudcover =  ofMap(parameters.num,0.0,800,0.0,1.0,true);
    float speed  = ofMap(parameters.speed,0.0,10.0,0.005,0.15,true);
    
    auto background = AppManager::getInstance().getGuiManager().getColor(0);
    auto color = AppManager::getInstance().getGuiManager().getColor(1);
    
    width = AppManager::getInstance().getSettingsManager().getAppWidth();
    height = AppManager::getInstance().getSettingsManager().getAppHeight();

    m_fbo.begin();
    ofClear(0);
    //ofBackground(background);
    if(ofIsGLProgrammableRenderer())
    {
        m_shadertoy.begin();
        m_shadertoy.setUniform3f("iResolution", width, height, 0.0);
        m_shadertoy.setUniform3f("iColor",color.r/255.0,color.g/255.0,color.b/255.0);
        m_shadertoy.setUniform3f("skycolour1",background.r/255.0,background.g/255.0,background.b/255.0);
        m_shadertoy.setUniform3f("skycolour2",background.r/255.0,background.g/255.0,background.b/255.0);
        m_shadertoy.setUniform1f("iTime", ofGetElapsedTimef()*speed);
        m_shadertoy.setUniform1f("cloudcover", cloudcover);
        ofDrawRectangle(0, 0, width, height);
        m_shadertoy.end();
    }
    
    else{
        
        m_shader.begin();
        m_shader.setUniform3f("iColor",color.r/255.0,color.g/255.0,color.b/255.0);
        m_shader.setUniform3f("iResolution", width, height, 0.0);
        m_shader.setUniform1f("iTime", ofGetElapsedTimef()*speed);
        m_shader.setUniform1f("cloudcover", cloudcover);
        m_shader.setUniform1f("speed", speed);
        ofDrawRectangle(0, 0, width, height);
        m_shader.end();
    }
    
    
     m_fbo.end();
    
    width = AppManager::getInstance().getSettingsManager().getAppWidth();
    height = AppManager::getInstance().getSettingsManager().getAppHeight();
    m_fbo.draw(0,0, width, height);
}


void CloudScene::willFadeIn() {
    AppManager::getInstance().getGuiManager().loadPresetsValues(getName());
    ofLogNotice("CloudScene::willFadeIn");    
}

void CloudScene::willDraw() {
    ofLogNotice("CloudScene::willDraw");
}

void CloudScene::willFadeOut() {
    AppManager::getInstance().getGuiManager().savePresetsValues(getName());
    ofLogNotice("CloudScene::willFadeOut");
}

void CloudScene::willExit() {
     ofLogNotice("CloudScene::willExit");
}

