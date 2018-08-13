/*
 *  Led.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */


#pragma once

#include "ofMain.h"
#include "BasicVisual.h"


class Led: public BasicVisual {
    
    static const float SIZE;
    
    public:
    
        Led(const ofPoint& position,const ofPoint& map_position, int id);
    
        virtual ~Led();
    
        //bool operator<(const Led &l1, const Led &l2);
    
        //bool operator<(const Led &led) const { return this->getId() < led.getId(); }
    
       // bool operator<(const Led &led) const;
    
        void draw();
    
        void draw(int width, int height);
    
        int getId() const {return m_id;}
    
        void normalize(const ofRectangle& boundingBox);
    
        void setPixelColor(ofPixelsRef pixels);
    
        const ofPoint& getMapPosition() const {return m_mapPosition;}
    

    private:
            
        int m_id;
        ofPoint m_mapPosition; 

};







