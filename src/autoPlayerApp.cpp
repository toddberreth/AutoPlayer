/*
 * Insight Engine
 * https://github.com/toddberreth/AutoPlayer
 *
 * Copyright 2015, Todd Berreth
 *
 * Licensed under the MIT license:
 * http://www.opensource.org/licenses/MIT
 */

#include "autoPlayerApp.h"

//--------------------------------------------------------------
void autoPlayerApp::setup(){
    
    ofSetBackgroundColor(0,0,0);
    
    data = new autoPlayerData();
    
    ofSetFrameRate(FRAME_RATE);
    ofSetVerticalSync(VERTICAL_SYNC);
    ofSetLogLevel(LOG_LEVEL);

    loadConfig();
    
    if (data->bFullScreen){ofSetFullscreen(data->bFullScreen);}
    else {ofSetWindowShape(data->windowWidth, data->windowHeight);ofSetWindowPosition(0, 0);}
    
    bMouseMoved = false; mouseTimer = 0;
}

//--------------------------------------------------------------
void autoPlayerApp::exit(){
    
    if ((int)events.size() > 0){ for (int i = 0; i < events.size(); i++) delete events[i];}
}

//--------------------------------------------------------------
void autoPlayerApp::update(){

    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {(*it)->update();}
    data->update();
    ofSoundUpdate();
    
    countPlayingMedia();
    
    mouseTimer++;
    if (bMouseMoved == true) {mouseTimer = 0; bMouseMoved = false; ofShowCursor();}
    if (mouseTimer > MOUSE_HIDE*FRAME_RATE) ofHideCursor();
}

//--------------------------------------------------------------
void autoPlayerApp::draw(){

    ofBackground(data->backgroundColor.r*data->globalAlpha,data->backgroundColor.g*data->globalAlpha,data->backgroundColor.b*data->globalAlpha);
    ofSetColor(255);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ofPushMatrix();
    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {(*it)->draw();}
    ofPopMatrix();
    glDisable(GL_BLEND);
    if (data->bShowData) data->draw();
}

//--------------------------------------------------------------
void autoPlayerApp::keyPressed(int key){

    if (key == 'f'){
        data->bFullScreen = !data->bFullScreen; ofSetFullscreen(data->bFullScreen);
        if (!data->bFullScreen){
            ofSetWindowShape(data->origWindowWidth, data->origWindowHeight);
            ofSetWindowPosition(0, 0);
        }
    }
    
    if (key == 'a') {data->bAudioOn = !data->bAudioOn; data->globalVolume = 0;}
    if (key == 's') data->bShowData = !data->bShowData;
}

//--------------------------------------------------------------
void autoPlayerApp::mouseMoved(int x, int y ){

    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {(*it)->mouseMoved(x,y);}
    bMouseMoved = true;
}

//--------------------------------------------------------------
void autoPlayerApp::countPlayingMedia(){
    
    data->videosPlaying = 0;
    data->soundsPlaying = 0;
    
    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {
        if ((*it)->video.isPlaying()) data->videosPlaying++;
        if ((*it)->sound.isPlaying()) data->soundsPlaying++;
    }
}

//--------------------------------------------------------------
void autoPlayerApp::keyReleased(int key){}
//--------------------------------------------------------------
void autoPlayerApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void autoPlayerApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void autoPlayerApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void autoPlayerApp::mouseEntered(int x, int y){}
//--------------------------------------------------------------
void autoPlayerApp::mouseExited(int x, int y){}
//--------------------------------------------------------------
void autoPlayerApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void autoPlayerApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void autoPlayerApp::dragEvent(ofDragInfo dragInfo){}
