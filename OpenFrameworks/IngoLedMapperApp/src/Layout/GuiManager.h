/*
 *  GuiManager.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxDatGui.h"


//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *    \brief Class managing the application´s grapical user interface (GUI)
 *    \details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const string PRESETS_PREFIX;
    static const int GUI_WIDTH;
    
public:
    
    //! Constructor
    GuiManager();
    
    //! Destructor
    ~GuiManager();
    
    //! Set-up the gui
    void setup();
    
    //! update the gui
    void update();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues(string path = "");
    
    void loadGuiValues(string path = "");
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return m_gui.getWidth();}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    void onToggleEvent(ofxDatGuiToggleEvent e);
    
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    void setModelPath(const string& path) {m_modelPath = ofToDataPath(path);}
    
    void setVideoPath(const string& path) {m_videoPath = ofToDataPath(path);}
    
    float getLedsSize() const {return m_ledsSize;}
    
    void savePresetsValues(const string& sceneName);
    
    void loadPresetsValues(const string& sceneName);
    
    void setSceneTransitionTime(float value) {m_sceneTransitionTime = value;}
    
    void onSceneChange(const string& sceneName);
    
    void onSceneChange(int sceneIndex);
    
    const ofColor& getColor(int index);
    
    void setColor(string name, ofColor& color);
    
    void onResetColors();
    
    float getBlur() const{return m_blur;}
    
private:
    
    void setupGuiParameters();
    
    void setupScenesGui();
    
    void setupParticlesGui();
    
    void setupPaletteGui();
    
    void setupLayoutGui();
    
    void setupFileGui();
    
    void setupLevelsGui();
    
    void setupLedsGui();
    
    void drawRectangle();
    
    void drawGui();
    
    void setupGuiEvents();
    
    
private:
    
    ofxDatGui            m_gui;
    
    ofParameter<float>    m_guiFPS;
    
    
    ofParameterGroup        m_parameters;
    ofParameterGroup        m_presets;
    
    ofParameter<float>      m_sceneTransitionTime;
    ofParameter<float>      m_sceneTimer;
    
    ofParameter<string>     m_modelPath;
    ofParameter<string>     m_videoPath;
    string                  m_projectPath;
    
    ofParameter<float>      m_blur;
    ofParameter<float>      m_contrast;
    ofParameter<float>      m_saturation;
    ofParameter<float>      m_brightness;
    ofParameter<float>      m_gamma;
    ofParameter<float>      m_minInput;
    ofParameter<float>      m_maxInput;
    ofParameter<float>      m_minOutput;
    ofParameter<float>      m_maxOutput;
    
    ofParameter<float>      m_ledsSize;
    ofParameter<float>      m_videoSpeed;
    
    
    ofParameter<float>      m_particlesDirection;
    ofParameter<float>      m_particlesDirectionMag;
    ofParameter<float>      m_particlesSpeed;
    ofParameter<float>      m_particlesSize;
    ofParameter<float>      m_particlesFade;
    ofParameter<int>        m_particlesNum;
    ofParameter<float>      m_particlesVectSpeed;
    ofParameter<float>      m_particlesRandomness;
    ofParameter<float>      m_particlesBlur;
    
    
    vector<ofParameter<int> > m_colorHexVector;
    vector<ofColor >          m_colors;
    
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


