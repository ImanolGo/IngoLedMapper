/*
 *  KeyboardManager.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */




#pragma once

#include "ofMain.h"
#include "Manager.h"

//========================== class KeyboardManager =======================================
//==============================================================================
/** \class KeyboardManager KeyboardManager.h
 *	\brief class for managing the keyboard events
 *	\details It reads all the keyboard events and create the proper application events
 */


class KeyboardManager: public Manager
{

public:
    //! Constructor
    KeyboardManager();

    //! Destructor
    virtual ~KeyboardManager();

    //! setups the day watcher
    void setup();

    //! Callback functions when key is pressed
    virtual void keyPressed(ofKeyEventArgs &e);
    
    //! Callback functions when key is released
    virtual void keyReleased(ofKeyEventArgs &e);


private:
    
    //! Adds the events to be listen to
    virtual void addListeners();
    
private:
    
    
    bool m_commandPressed;
    

};

