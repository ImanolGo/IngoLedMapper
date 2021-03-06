#include "ofMain.h"
#include "IngoLedMapperApp.h"

//========================================================================
int main( ){

	//ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);    // <--- ofxShadertoy NEEDS the GL Programmable Renderer
    settings.width = 1024;
    settings.height = 768;
    ofCreateWindow(settings);       // <-------- setup the GL context
    
    
	ofRunApp( new IngoLedMapperApp());

}

