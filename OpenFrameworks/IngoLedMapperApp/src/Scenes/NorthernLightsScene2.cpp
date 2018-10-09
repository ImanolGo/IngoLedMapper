/*
 *  NorthernLightsScene2.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 21/09/18.
 *
 */


#include "NorthernLightsScene2.h"
#include "AppManager.h"


NorthernLightsScene2::NorthernLightsScene2(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

NorthernLightsScene2::~NorthernLightsScene2()
{
    //Intentionally left empty
}


void NorthernLightsScene2::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupShader();
}

void NorthernLightsScene2::setupShader()
{
    
     ofLogNotice(getName() + "::setupShaders");
    
    ofImage img;
    img.load("images/textures/iChannel0.png");
    ///*
    m_channel1.allocate(img.getWidth(), img.getHeight(), GL_RGBA, false); // fourth parameter is false to avoid generation of a GL_TEXTURE_2D_RECTANGLE texture - we don't want this.
    m_channel1.loadData(img.getPixels());
    m_channel1.generateMipmap();
    m_channel1.setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    
    ofImage img2;
    img2.load("images/textures/iChannel1.png");
    ///*
    m_channel2.allocate(img2.getWidth(), img2.getHeight(), GL_RGBA, false); // fourth parameter is false to avoid generation of a GL_TEXTURE_2D_RECTANGLE texture - we don't want this.
    m_channel2.loadData(img2.getPixels());
    m_channel2.generateMipmap();
    m_channel2.setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/AuroraBorealis.frag");
         m_shadertoy.setTexture(0, m_channel1);
         m_shadertoy.setTexture(1, m_channel2);
    }
    else{
        m_shader.load("shaders/shadersGL2/AuroraBorealis");
    }
}



void NorthernLightsScene2::update()
{
    //Empty
}

void NorthernLightsScene2::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawShader();
    ofDisableAlphaBlending();
   
}


void NorthernLightsScene2::drawShader()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto parameters = AppManager::getInstance().getParticlesManager().getParameters();
    
    //float speed  = ofMap(parameters.speed,0.0,5.0,0.0,1.0,true);
    //auto color = AppManager::getInstance().getGuiManager().getColor(1);
    
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
        m_shader.setUniform2f("texResolution", m_channel1.getWidth(), m_channel1.getHeight());
        m_channel1.bind();
        m_shader.setUniformTexture("iChannel0", m_channel1, 1);
        m_shader.setUniformTexture("iChannel1", m_channel2, 2);
        
        ofDrawRectangle(0,0, width, height );
        
        m_shader.end();
    }
}


void NorthernLightsScene2::willFadeIn() {
    AppManager::getInstance().getGuiManager().loadPresetsValues(getName());
    ofLogNotice("NorthernLightsScene2::willFadeIn");
}

void NorthernLightsScene2::willDraw() {
    ofLogNotice("NorthernLightsScene2::willDraw");
}

void NorthernLightsScene2::willFadeOut() {
    AppManager::getInstance().getGuiManager().savePresetsValues(getName());
    ofLogNotice("NorthernLightsScene2::willFadeOut");
}

void NorthernLightsScene2::willExit() {
     ofLogNotice("NorthernLightsScene2::willExit");
}

