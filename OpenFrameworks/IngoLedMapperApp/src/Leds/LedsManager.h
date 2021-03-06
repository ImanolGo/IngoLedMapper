/*
 *  LedsManager.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */



#pragma once

#include "Manager.h"
#include "Led.h"

//========================== class LedsManager ==============================
//============================================================================
/** \class LedsManager LedsManager.h
 *	\brief Class managing the LEDs
 *	\details It controls the postion and color of the LEDs
 */


class LedsManager: public Manager
{
    static const int LEDS_PER_STRIPE;
    
    public:
    
        typedef vector<int> IntVector;
        typedef vector< ofPtr<Led> > LedVector;

    
    public:

        //! Constructor
        LedsManager();

        //! Destructor
        ~LedsManager();

        //! Setup the Halo Manager
        void setup();

        //! Update the Led Manager
        void update();
    
        //! Draw the Led Manager
        void draw();
    
        const LedVector& getLeds() const {return m_leds;}
    
        void setPixels(ofPixelsRef pixels);
    
        void setLedColors(ofPixelsRef pixels);
    
        const ofPoint getMin() const {return m_minPos;}
    
        const ofPoint getMax() const {return m_maxPos;}
    
        bool loadLeds();
    
        bool load(string& path);
    
        void loadTest();
    
        bool isNewFrame() const {return m_isNewFrame;}
    
        void setSize(float& value);
 
    
    private:
    
    
        void setupLeds();
    
        void readLedsPosition();
    
        void createLedPositions();
    
        void sortLeds();
    
        void normalizeLeds();
    
        bool getIs3D();
    
        void  centreLeds();
    
        void readLedsPositionFromGroup(const string& groupName, int& id, int numberOfSections);
    
        void readLasersPositionFromGroup(const string& groupName, int& id, vector<int>& sections);
    
        bool parseLedLine(string& line, ofPoint& position);
    
        void createLed(const ofPoint& position, const ofPoint& map_position, int& id);
    
        void removeCharsFromString( string &str, char* charsToRemove );
    
        bool isValidFile(const string& path);
    
    

    private:
    
        LedVector          m_leds;
        ofPoint            m_minPos;
        ofPoint            m_maxPos;
        bool               m_isNewFrame;
        string             m_ledsFilePath;
        bool               m_is3D;
    
};



