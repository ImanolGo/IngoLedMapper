/*
 *  IngoLedMapperApp.cpp
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */



#include "AppManager.h"

#include "IngoLedMapperApp.h"

//--------------------------------------------------------------
void IngoLedMapperApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void IngoLedMapperApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void IngoLedMapperApp::draw(){
    AppManager::getInstance().draw();
}

void IngoLedMapperApp::exit()
{
    ofLogNotice() <<"IngoLedMapperApp::exit";

}

//--------------------------------------------------------------
void IngoLedMapperApp::keyPressed(int key){

}

//--------------------------------------------------------------
void IngoLedMapperApp::keyReleased(int key){

}

//--------------------------------------------------------------
void IngoLedMapperApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void IngoLedMapperApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void IngoLedMapperApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void IngoLedMapperApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void IngoLedMapperApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);


}

//--------------------------------------------------------------
void IngoLedMapperApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void IngoLedMapperApp::dragEvent(ofDragInfo dragInfo){

}
