/*
 *  NoiseScene.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#include "NoiseScene.h"
#include "AppManager.h"


NoiseScene::NoiseScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

NoiseScene::~NoiseScene()
{
    //Intentionally left empty
}


void NoiseScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupNoiseShader();
    this->setupGradient();
}

void NoiseScene::setupNoiseShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/Noise.frag");
    }
    else{
        m_shader.load("shaders/shadersGL2/Noise");
    }
    
}

void NoiseScene::setupGradient()
{
    m_gradient.reset();
    ofColor color = AppManager::getInstance().getGuiManager().getColor(0);
    m_gradient.addColor(color);
    color = AppManager::getInstance().getGuiManager().getColor(1);
    m_gradient.addColor(color);
//    color = AppManager::getInstance().getGuiManager().getColor(2);
//    m_gradient.addColor(color);
//    color = AppManager::getInstance().getGuiManager().getColor(3);
//    m_gradient.addColor(color);
//    color = AppManager::getInstance().getGuiManager().getColor(4);
//    m_gradient.addColor(color);
//    color = AppManager::getInstance().getGuiManager().getColor(5);
//    m_gradient.addColor(color);
    
//    m_gradient.addColor( ofColor::red );
//    m_gradient.addColor( ofColor::green );
//    m_gradient.addColor( ofColor::yellow );

}


void NoiseScene::update()
{
    //Empty
}

void NoiseScene::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawNoise();
    ofDisableAlphaBlending();
   
}


void NoiseScene::drawNoise()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto parameters = AppManager::getInstance().getParticlesManager().getParameters();
    
    float grain =  ofMap(parameters.size,200.0,0.0,0.0,10.0,true);
    float speed  = ofMap(parameters.speed,0.0,10.0,0.0,2.0,true);
    float colorPct  = ofMap(sin(ofGetElapsedTimef()*speed),-1.0,1.0,0.0,1.0,true);
    
    //auto color = AppManager::getInstance().getGuiManager().getColor(1);

    ofColor color = m_gradient.getColorAtPercent(colorPct);
    
    if(ofIsGLProgrammableRenderer())
    {
        m_shadertoy.begin();
        m_shadertoy.setUniform3f("iResolution", width, height, 0.0);
        m_shadertoy.setUniform3f("iColor",color.r/255.0,color.g/255.0,color.b/255.0);
        m_shadertoy.setUniform1f("iTime", ofGetElapsedTimef()*speed);
        m_shadertoy.setUniform1f("inoise_grain", grain);
            ofDrawRectangle(0, 0, width, height);
        m_shadertoy.end();
    }
    
     else{
         
        m_shader.begin();
        m_shader.setUniform3f("iColor",color.r/255.0,color.g/255.0,color.b/255.0);
        m_shader.setUniform3f("iResolution", width, height, 0.0);
        m_shader.setUniform1f("iTime", ofGetElapsedTimef()*speed);
        m_shader.setUniform1f("inoise_grain", grain);
            ofDrawRectangle(0, 0, width, height);
        m_shader.end();
     }
    
    
   // m_gradient.drawDebug(0,0, width, height);
}


void NoiseScene::willFadeIn() {
    AppManager::getInstance().getGuiManager().loadPresetsValues(getName());
    ofLogNotice("NoiseScene::willFadeIn");
    this->setupGradient();
}

void NoiseScene::willDraw() {
    ofLogNotice("NoiseScene::willDraw");
}

void NoiseScene::willFadeOut() {
    AppManager::getInstance().getGuiManager().savePresetsValues(getName());
    ofLogNotice("NoiseScene::willFadeOut");
}

void NoiseScene::willExit() {
     ofLogNotice("NoiseScene::willExit");
}

