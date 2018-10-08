/*
 *  SceneManager.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 15/08/18.
 *
 */



#include "ofMain.h"


#include "SceneManager.h"
#include "scenes.h"
#include "AppManager.h"

SceneManager::SceneManager(): Manager(), m_alpha(-1), m_transitionTime(1.0), m_activeScenes(true), m_timerOn(true)
{
	//Intentionally left empty
}


SceneManager::~SceneManager()
{
    ofLogNotice() <<"SceneManager::Destructor";
}


void SceneManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();

    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    this->createScenes();
    this->setupFbo();
    this->setupLevels(width,height);
    //this->setupTimer();
   // this->initializeSceneList();

    ofLogNotice() <<"SceneManager::initialized";

}


void SceneManager::createScenes()
{
    m_mySceneManager.setTransitionFade();
	//m_mySceneManager.setTransitionDissolve();
    
    ofPtr<ofxScene> scene;
    
    
    //Create HERBSTMORGEN Scene
    scene = ofPtr<ofxScene> (new VectorFieldScene("HERBSTMORGEN"));
    m_mySceneManager.addScene(scene);
    
    //Create AUTUMN Scene
    scene = ofPtr<ofxScene> (new AutumnScene("AUTUMN"));
    m_mySceneManager.addScene(scene);
    
    //Create BERGEN
    scene = ofPtr<ofxScene> (new NoiseScene("BERGEN"));
    m_mySceneManager.addScene(scene);
    
    //Create STERNE
//    auto vectorScene = ofPtr<VectorFieldScene> (new VectorFieldScene("STERNE"));
//    vectorScene->setAdditiveBlend(true);
//    m_mySceneManager.addScene(vectorScene);

    //Create NORDLICHT
    scene = ofPtr<ofxScene> (new NorthernLightsScene("NORDLICHT"));
    //scene = ofPtr<ofxScene> (new VectorFieldScene("LOVED"));
    m_mySceneManager.addScene(scene);
    
    //Create NORDLICHT
    scene = ofPtr<ofxScene> (new NorthernLightsScene2("NORDLICHT2"));
    //scene = ofPtr<ofxScene> (new VectorFieldScene("LOVED"));
    m_mySceneManager.addScene(scene);
    
    //Create STERNE
    scene = ofPtr<ofxScene> (new StarsScene("STERNE"));;
    m_mySceneManager.addScene(scene);
    
    //Create GRUEN
    scene = ofPtr<ofxScene> (new FlatScene("GRUEN"));
    m_mySceneManager.addScene(scene);
    
//    //Create Heart Video Scene
//    scene = ofPtr<ofxScene> (new VideoScene("TEST_VIDEO"));
//    m_mySceneManager.addScene(scene);
//
    //Create Blank Scene
    scene = ofPtr<ofxScene> (new BlankScene());
    m_mySceneManager.addScene(scene);
    

   // m_mySceneManager.run(width, height);
    this->onTransitionTimeChange(m_transitionTime);
}

void SceneManager::run()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    m_mySceneManager.run(width, height);
}

void SceneManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    //float width = ofGetWidth();
    //float height = ofGetHeight();

    m_fbo.allocate(width, height, GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void SceneManager::setupLevels(float width, float height)
{
    if( m_levels.setup(width,height))
    {
         ofLogNotice() <<"SceneManager::setupLevels << SUSCCESSFUL";
    }
    else{
         ofLogNotice() <<"SceneManager::setupLevels << UNSUSCCESSFUL";
    }
   
}

void SceneManager::setupTimer()
{
    //auto time = AppManager::getInstance().getSettingsManager().getSceneTimer();
    m_sceneTimer.setup(10000 );
    m_sceneTimer.start( false ) ;
    ofAddListener( m_sceneTimer.TIMER_COMPLETE , this, &SceneManager::sceneTimerCompleteHandler ) ;
    
    ofLogNotice() <<"SceneManager::setupTimer << Time = : " << time << "s";
}

void SceneManager::onChangeSceneDuration(float& value)
{
    m_sceneTimer.setup( value*1000 );
    if(m_timerOn){
        m_sceneTimer.start( false, true ) ;
    }
    ofLogNotice() <<"SceneManager::setupTimer << Time = : " << time << "s";
}




void SceneManager::update()
{
    this->updateScenes();
    this->updateFbo();
    //this->updateTimer();
}

void SceneManager::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        ofPushStyle();
        ofSetColor(255);
        ofEnableAlphaBlending();
        m_levels.begin();
            m_mySceneManager.draw();
        m_levels.end();
        m_levels.draw();
        ofDisableAlphaBlending();
        ofPopStyle();
    m_fbo.end();
    
    
    if(AppManager::getInstance().getLayoutManager().getDrawMode() == DRAW_SCENES)
    {
        return;
    }
    
    ofPixels pixels;
    m_fbo.readToPixels(pixels);
    AppManager::getInstance().getLedsManager().setPixels(pixels);
}


void SceneManager::updateScenes()
{
    m_mySceneManager.update();
}

void SceneManager::updateTimer()
{
    m_sceneTimer.update();
}


void SceneManager::draw()
{
//    ofEnableAlphaBlending();
//    m_fbo.draw(0,0);
//    ofDisableAlphaBlending();
   
    ofEnableAlphaBlending();
    
    string name = "Scenes";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = m_fbo.getWidth()/ m_fbo.getHeight();
    float height = rect->getHeight();
    float width = height*ratio;
    
    if( width > rect->getWidth() ){
        width = rect->getWidth();
        height = width/ratio;
    }
    
    float x = rect->getWidth()*0.5 - width*0.5;
    float y = rect->getHeight()*0.5 - height*0.5;
    
    ofSetColor(0);
    ofDrawRectangle(0, 0,rect->getWidth(), rect->getHeight());
    ofSetColor(255);
    m_fbo.draw(x,y, width, height);
    
    ofDisableAlphaBlending();
}

void SceneManager::draw(const ofRectangle& rect)
{
    m_fbo.draw(rect.x,rect.y,rect.width,rect.height);
}


void SceneManager::changeScene(string sceneName)
{
    m_mySceneManager.changeScene(sceneName);
    m_currentSceneName = sceneName;
    
}

void SceneManager::changeScene(int sceneIndex)
{
    m_mySceneManager.changeScene(sceneIndex);
    m_currentSceneName = this->getSceneName(sceneIndex);
}


void SceneManager::onTransitionTimeChange(float& value)
{
   m_mySceneManager.setSceneDuration(value,value);
}

string SceneManager::getSceneName(int sceneIndex)
{
    string name = "";
    if(sceneIndex < 0 || sceneIndex >= m_mySceneManager.scenes.size()){
        return name;
    }
    
    return m_mySceneManager.scenes[sceneIndex]->getName();
   
}

int SceneManager::getIndex(const string& sceneName)
{
    
    for(int i = 0; i< m_mySceneManager.scenes.size(); i++){
        if(m_mySceneManager.scenes[i]->getName() == sceneName){
            return i;
        }
    }
    
    return -1;
}

void SceneManager::sceneTimerCompleteHandler( int &args )
{
    if(!m_timerOn){
        return;
    }
    
    this->nextScene();
}

void SceneManager::setTimerOn(bool value)
{
    m_timerOn = value;
    if(m_timerOn){
        m_sceneTimer.start(false);
    }
}

void SceneManager::nextScene()
{
    m_sceneTimer.start(false);
    
//    if(m_sceneList.empty()){
//        //this->initializeSceneList();
//    }
    
    string sceneName = m_sceneList.back();  m_sceneList.pop_back();
    AppManager::getInstance().getGuiManager().onSceneChange(sceneName);
    
    ofLogNotice() <<"SceneManager::nextScene << Change Scene: " << sceneName;
}

void SceneManager::stopScenes()
{
    m_sceneTimer.stop();
    ofLogNotice() <<"SceneManager::nextScene << Stop Scenes!";
}

void SceneManager::toggleActiveScenes()
{
    this->setActiveScenes(!m_activeScenes);
}

void SceneManager::setActiveScenes(bool value)
{
    m_activeScenes = value;
    
    ofLogNotice() <<"SceneManager::setActiveScenes: " << value;
    if(m_activeScenes){
        this->nextScene();
        AppManager::getInstance().getGuiManager().onSceneChange(0);
    }
    else{
        AppManager::getInstance().getGuiManager().onSceneChange("BLANK");
        this->stopScenes();
    }
}





