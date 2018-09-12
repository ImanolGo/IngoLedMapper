/*
 *  KeyboardManager.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */


#include "KeyboardManager.h"
#include "AppManager.h"


KeyboardManager::KeyboardManager(): Manager(), m_commandPressed(false)
{
    //Intentionally left empty
}

KeyboardManager::~KeyboardManager()
{
   ofLogNotice() << "KeyboardManager::destructor";
}


//--------------------------------------------------------------

void KeyboardManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"KeyboardManager::initialized" ;
    this->addListeners();
}

void KeyboardManager::addListeners()
{
    ofRegisterKeyEvents(this); // this will enable the InteractiveVisual class to listen to the key events.
}

void KeyboardManager::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    
    ofLogVerbose() <<"KeyboardManager::keyPressed-> " + ofToString(key);
    
  
    if(key == 4352) { //Command
        m_commandPressed = true;
        ofLogVerbose() <<"KeyboardManager::keyPressed->  CMD Pressed";
    }
    
    if(m_commandPressed)
    {
        ofLogVerbose() <<"KeyboardManager::keyPressed-> " + ofToString(key);
        if(key == 'f' || key == 'F')
        {
            //ofLogVerbose() <<"KeyboardManager::keyReleased-> " + ofToString(key);
            AppManager::getInstance().getLayoutManager().onToggleFullscreen();
        }
    }
    
//    if(key == 'g' || key == 'G') {
//        AppManager::getInstance().getGuiManager().toggleGui();
//    }
//    
//    if(key == 'f' || key == 'F') {
//        AppManager::getInstance().getLayoutManager().onToggleFullscreen();
//    }
//    
//    
//    if(key == ' ') {
//        AppManager::getInstance().getLayoutManager().toggleDrawMode();
//        if(AppManager::getInstance().getLayoutManager().getDrawMode() == 0){
//             AppManager::getInstance().getGuiManager().showGui(true);
//        }
//        else{
//            AppManager::getInstance().getGuiManager().showGui(false);
//        }
//    }
    
}

void KeyboardManager::keyReleased(ofKeyEventArgs &e)
{
    int key = e.key;
    
    if(key == 4352) {
        m_commandPressed = false;
        ofLogVerbose() <<"KeyboardManager::keyReleased-> CMD Released";
    }
    
    //ofLogVerbose() <<"KeyboardManager::keyReleased-> " + ofToString(key);
    
}










