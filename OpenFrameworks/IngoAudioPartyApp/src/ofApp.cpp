#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetDataPathRoot("../Resources/data/");
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofBackground(54, 54, 54);
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    soundStream.printDeviceList();
    
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter    = 0;
    drawCounter        = 0;
    smoothedVol     = 0.0;
    scaledVol        = 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    soundStream.start();
    
    
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);

}

//--------------------------------------------------------------
void ofApp::update()
{
    
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    
    scaledVol = ofClamp(scaledVol, 0.0, 1.0);

}

//--------------------------------------------------------------
void ofApp::draw()
{
    // display instructions
    string buf;
    buf = "sending osc messages to host: " + string(HOST) + ", port:" + ofToString(PORT);
    ofDrawBitmapString(buf, 10, 20);
    
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofDrawCircle(300, 300, scaledVol * 190.0f);
    
    buf = "Volume: " + ofToString(scaledVol);
    ofDrawBitmapString(buf, 10, 50);
    
    ofxOscMessage m;
    m.setAddress("/ppx/energy");
    m.addFloatArg(scaledVol);
    sender.sendMessage(m, false);
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if(key == '1'){
        ofxOscMessage m;
        m.setAddress("/ppx/changeMode");
        m.addIntArg(0);
        sender.sendMessage(m, false);
    }
    
   else  if(key == '2'){
        ofxOscMessage m;
        m.setAddress("/ppx/changeMode");
        m.addIntArg(1);
        sender.sendMessage(m, false);
    }
    
    
    else if(key == '3'){
        ofxOscMessage m;
        m.setAddress("/ppx/changeMode");
        m.addIntArg(2);
        sender.sendMessage(m, false);
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]        = input[i*2]*0.5;
        right[i]    = input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.97;
    smoothedVol += 0.03 * curVol;
    
    bufferCounter++;
    
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
