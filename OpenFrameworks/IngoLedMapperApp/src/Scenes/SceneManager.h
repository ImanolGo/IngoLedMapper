/*
 *  SceneManager.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#pragma once

#include "Manager.h"
#include "ofxSceneManager.h"
#include "ofxSimpleTimer.h"
#include "ofxPSLevels.h"


//========================== class SceneManager ==============================
//============================================================================
/** \class SceneManager SceneManager.h
 *	\brief Class managing the scenes of the application
 *	\details It creates and manages all the scenes 
 */


class SceneManager: public Manager
{
    
public:

    //! Constructor
    SceneManager();

    //! Destructor
    ~SceneManager();

    //! Set-up the scenes
    void setup();
    
    //! Update the scenes
    void update();
    
    //! Draw the sceneManager
    void draw();
    
    //! Draw the sceneManager within a window rectangle
    void draw(const ofRectangle& rect);
    
    //! Run the sceneManager
    void run();
    
    //! Changes the scene according to a name
    void changeScene(string sceneName);
    
    //! Changes the scene according to a index
    void changeScene(int sceneIndex);
    
    //! changes transition time
    void onTransitionTimeChange(float& value);
    
    //! Returns the number of total secenes
    int getNumberScenes(){return m_mySceneManager.scenes.size();};
    
    //! Returns the name of a scene given the index
    string getSceneName(int sceneIndex);
    
    //! Returns the index a scene given a name. It returns -1 if it doesn't find any
    int getIndex(const string& sceneName);
    
    const ofFbo& getFbo() {return m_fbo;}
    
    void sceneTimerCompleteHandler( int &args ) ;
    
    void onChangeSceneDuration(float& value);
    
    string getCurrentSceneName() const {return m_currentSceneName;}
    
    void toggleActiveScenes();
    
    void setActiveScenes(bool value);
    
    void setTimerOn(bool value);
    
    void setContrast(float& value) {m_levels.contrast = value;}
    
    void setSaturation(float& value) {m_levels.saturation = value;}
    
    void setBrightness(float& value) {m_levels.brightness = value;}
    
    void setGamma(float& value) {m_levels.gamma = value;}
    
    void setMinInput(float& value) {m_levels.minInput = value;}
    
    void setMaxInput(float& value) {m_levels.maxInput = value;}
    
    void setMinOutput(float& value) {m_levels.minOutput = value;}
    
    void setMaxOutput(float& value) {m_levels.maxOutput = value;}
    
private:
    
    //! Create the scenes
    void createScenes();
    
    //! Set up the fbo that saves the texture.
    void setupFbo();
    
    //! Set up the scene timer
    void setupTimer();
    
    void setupLevels(float width, float height);
    
     //! Initializes the scene list
    //void initializeSceneList();
    
    //! Update the scene timer
    void updateTimer();
    
    //! updates all the scenes
    void updateScenes();
    
    //! updates the fbo
    void updateFbo();

    //! next scene
    void nextScene();
    
    //! stop scenes
    void stopScenes();

private:

    ofxSceneManager          m_mySceneManager;
    ofFbo                    m_fbo;
    ofxSimpleTimer           m_sceneTimer;
    float                    m_alpha;
    ofxPSLevels              m_levels;
    
    vector<string>           m_sceneList;
    string                   m_currentSceneName;
    float                    m_transitionTime;
    bool                     m_activeScenes;
    bool                     m_timerOn;
};

//==========================================================================


