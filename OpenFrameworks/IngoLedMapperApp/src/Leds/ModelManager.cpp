/*
 *  ModelManager.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */


#include "ofMain.h"

#include "ModelManager.h"
#include "AppManager.h"


ModelManager::ModelManager(): Manager()
{
	//Intentionally left empty
}


ModelManager::~ModelManager()
{
    ofLogNotice() <<"ModelManager::Destructor" ;
}



void ModelManager::setup()
{
	if(m_initialized)
		return;

    ofLogNotice() <<"ModelManager::initialized" ;

	Manager::setup();
    
    this->resetCamera();
}

void ModelManager::resetCamera()
{
    m_camera.reset();
    m_camera.setDistance(35);
    //m_camera.enableInertia();
    //m_camera.setAutoDistance(true);
}


void ModelManager::update()
{
    
}


void ModelManager::draw()
{
    string name = "Leds";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    
    ofRectangle rect2 = ofRectangle(rect->x, rect->y, 0, 0);
    
    ofBackgroundGradient( ofColor(100), ofColor(15), OF_GRADIENT_CIRCULAR );
    ofEnableDepthTest();
    //ofTranslate(-rect->x, 0, 0);
    m_camera.begin();
   // ofTranslate(-rect->x, 0, 0);
    this->drawLeds();
   // ofTranslate(rect->x, 0 , 0);
    m_camera.end();
    //ofTranslate(rect->x, 0 , 0);
    ofDisableDepthTest();
    
}

void ModelManager::drawLeds()
{
    float max = 100;
    auto ledVector = AppManager::getInstance().getLedsManager().getLeds();
    for (auto led: ledVector) {
        auto pos = led->getPosition();

        ofPushMatrix();
            ofTranslate(pos*max);
            led->draw();
        ofPopMatrix();
    }
}
