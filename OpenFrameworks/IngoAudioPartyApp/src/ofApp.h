#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

//#define HOST "localhost"
#define HOST "192.168.8.1"
#define PORT 2345

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        void audioIn(float * input, int bufferSize, int nChannels);
        
        vector <float> left;
        vector <float> right;
        vector <float> volHistory;
        
        int     bufferCounter;
        int     drawCounter;
        
        float smoothedVol;
        float scaledVol;
        
        ofSoundStream soundStream;
    
        ofTrueTypeFont font;
        ofxOscSender sender;
        ofxPanel gui;
     ofxFloatSlider volume;
		
};
