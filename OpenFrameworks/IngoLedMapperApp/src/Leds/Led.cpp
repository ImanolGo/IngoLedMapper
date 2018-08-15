/*
 *  Led.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */



#include "Led.h"
#include "AppManager.h"


const float Led::SIZE = 1.0;

Led::Led(const ofPoint& position, const ofPoint& map_position, int id): BasicVisual(position, SIZE, SIZE), m_id(id), m_mapPosition(map_position)
{
    m_color = ofColor::black;
    //ofLogNotice() <<  "Led::Sphere Resolution: " << ofGetSphereResolution();
    //ofSetSphereResolution(2);
}

Led::~Led()
{
    //Intentionaly left empty
}

void Led::draw()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(m_position);
    ofRotateX(m_rotation.x);
    ofRotateY(m_rotation.y);
    ofRotateZ(m_rotation.z);
    ofScale(m_scale.x, m_scale.y, m_scale.z);
    
    ofSetColor(m_color);
    ofFill();
    
        ofDrawSphere(0, 0, m_width);
    
    ofPopStyle();
    ofPopMatrix();
}


void Led::normalize(const ofRectangle& boundingBox)
{
    m_position.x = (m_position.x - boundingBox.getX()) / boundingBox.getWidth();
    m_position.y = (m_position.y - boundingBox.getY()) / boundingBox.getHeight();
}


void Led::setPixelColor(ofPixelsRef pixels)
{
    ofPoint min = AppManager::getInstance().getLedsManager().getMin();
    ofPoint max = AppManager::getInstance().getLedsManager().getMax();
    ofPoint pixelPos;
    
    pixelPos.x = ofMap(m_mapPosition.x, min.x, max.x, 0, pixels.getWidth()-1);
    pixelPos.y = ofMap(m_mapPosition.y, max.y, min.y, 0,  pixels.getHeight()-1);
    
    //ofLogNotice() <<  m_position.x ; ofLogNotice() <<  m_position.y;
    //ofLogNotice() <<  pixelPos.x ; ofLogNotice() <<  pixelPos.y;
   //ofLogNotice() << pixelPos.x;
    
    m_color = pixels.getColor((int)pixelPos.x, (int)pixelPos.y);
}

