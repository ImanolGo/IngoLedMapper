/*
 *  ColorGradientScene.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#include "ColorGradientScene.h"
#include "AppManager.h"


ColorGradientScene::ColorGradientScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

ColorGradientScene::~ColorGradientScene()
{
    //Intentionally left empty
}


void ColorGradientScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupShader();
    this->setupGradient();
    this->setupFbo();
    this->setupBlur();

}


void ColorGradientScene::setupShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/Sunrise.frag");
    }
    else{
        m_shader.load("shaders/shadersGL2/Sunrise");
    }
    
}

void ColorGradientScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void ColorGradientScene::setupGradient()
{
    m_gradient.reset();
    ofColor color = AppManager::getInstance().getGuiManager().getColor(0);
    m_gradient.addColor(color);
    color = AppManager::getInstance().getGuiManager().getColor(1);
    m_gradient.addColor(color);
}


void ColorGradientScene::setupBlur()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    ofFbo::Settings s;
    s.width = width;
    s.height = height;
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;
    
    m_blur.setup(s, false);
    m_blur.blurPasses = 3;
    m_blur.blurOffset = 0;
    m_blur.numBlurOverlays = 1;
    m_blur.blurOverlayGain = 255;
}



void ColorGradientScene::update()
{
    this->updateBlur();
}

void ColorGradientScene::updateBlur()
{
      auto parameters = AppManager::getInstance().getParticlesManager().getParameters();
    
    m_blur.blurOffset = 10*parameters.blur;
}

void ColorGradientScene::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawShader();
    ofDisableAlphaBlending();
   
}

void ColorGradientScene::drawGradient()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
     m_fbo.begin();
        ofClear(0);
        //m_gradient.drawDebug(0, height/4, width, height/2);
    
   
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
    
     m_fbo.end();
    
    m_fbo.draw(0, height/4, width, height/2);
    
}

void ColorGradientScene::drawShader()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto parameters = AppManager::getInstance().getParticlesManager().getParameters();
    
    float grain =  ofMap(parameters.size,200.0,0.0,0.0,10.0,true);
    float speed  = ofMap(parameters.speed,0.0,10.0,0.0,2.0,true);
    float colorPct  = ofMap(sin(ofGetElapsedTimef()*speed),-1.0,1.0,0.0,1.0,true);
    
    //auto color = AppManager::getInstance().getGuiManager().getColor(1);

    ofColor color = m_gradient.getColorAtPercent(colorPct);
    
    m_blur.beginDrawScene();
    
    ofClear(0);
    
//    m_shader.begin();
//    m_shader.setUniform3f("iColor",color.r/255.0,color.g/255.0,color.b/255.0);
//    m_shader.setUniform3f("iResolution", width, height, 0.0);
//    m_shader.setUniform1f("iTime", ofGetElapsedTimef()*speed);
//    m_shader.setUniform1f("inoise_grain", grain);
//    ofDrawRectangle(0, 0, width, height);
//    m_shader.end();
    this->drawGradient();
    
    m_blur.endDrawScene();
    m_blur.performBlur();
    m_blur.drawBlurFbo();
    
    
   // m_gradient.drawDebug(0,0, width, height);
}


void ColorGradientScene::willFadeIn() {
    AppManager::getInstance().getGuiManager().loadPresetsValues(getName());
    ofLogNotice("ColorGradientScene::willFadeIn");
    this->setupGradient();
}

void ColorGradientScene::willDraw() {
    ofLogNotice("ColorGradientScene::willDraw");
}

void ColorGradientScene::willFadeOut() {
    AppManager::getInstance().getGuiManager().savePresetsValues(getName());
    ofLogNotice("ColorGradientScene::willFadeOut");
}

void ColorGradientScene::willExit() {
     ofLogNotice("ColorGradientScene::willExit");
}

