#include "autoPlayerData.h"

autoPlayerData::autoPlayerData(){

	windowWidth = origWindowWidth = WINDOW_WIDTH_DEFAULT; windowHeight = origWindowHeight = WINDOW_HEIGHT_DEFAULT;
    bFullScreen = false; backgroundColor.set(0,0,0);
    
    playType = LOOP;
	
	bDisplayOn = bAudioOn = true; globalAlpha = 0; bShowData = false; globalVolume = 0;
	
	frameNumber = 0;
    
    playDurationSeconds = (float)PLAY_DURATION_MIN; playDurationFrames = (int)(PLAY_DURATION_MIN * FRAME_RATE);
	
	globalAlphaStep = globalVolumeStep = (float)1/(GLOBAL_TRANSITION*FRAME_RATE);
	
	videosPlaying = soundsPlaying = 0; videosPlayingMax = VIDEOS_PLAYING_MAX_DEFAULT; soundsPlayingMax = SOUNDS_PLAYING_MAX_DEFAULT;
	
	tiles_h = tiles_v = 1;
	
	inputType = OFF;
    
    message = "";
    
    randomAlphaMinTyp = RANDOM_ALPHA_MIN_TYP;   randomAlphaMaxTyp = RANDOM_ALPHA_MAX_TYP;
    randomSpeedMinTyp = RANDOM_SPEED_MIN_TYP;   randomSpeedMaxTyp = RANDOM_SPEED_MAX_TYP;
    randomVolumeMinTyp = RANDOM_VOLUME_MIN_TYP; randomVolumeMaxTyp = RANDOM_VOLUME_MAX_TYP;
    randomDurationMinTyp = RANDOM_DURATION_MIN_TYP; randomVolumeMaxTyp = RANDOM_DURATION_MAX_TYP;
}

autoPlayerData::~autoPlayerData(){}

void autoPlayerData::update(){
	
	frameNumber++;
	if ((frameNumber == playDurationFrames) && (playType == LOOP)) frameNumber = 0;
	if ((frameNumber == playDurationFrames) && (playType == ONCE)) exit(0);
	
	windowWidth = ofGetWindowWidth(); windowHeight = ofGetWindowHeight();
	
	if (bDisplayOn) globalAlpha+=globalAlphaStep; else globalAlpha-=globalAlphaStep;
	globalAlpha = ofClamp(globalAlpha, 0, 1);
	
	if (bAudioOn) globalVolume+=globalVolumeStep; else globalVolume-=globalVolumeStep;
	globalVolume = ofClamp(globalVolume, 0, 1);
	
	if (bPlayTransition){
		if ((bDisplayOn) && (frameNumber == (playDurationFrames - (GLOBAL_TRANSITION*FRAME_RATE)))&& (playType != ENDLESS)) bDisplayOn = false;
		if ((!bDisplayOn) && (frameNumber == 0)) bDisplayOn = true;
		
		if ((bAudioOn) && (frameNumber == (playDurationFrames - (GLOBAL_TRANSITION*FRAME_RATE)))&& (playType != ENDLESS)) bAudioOn = false;
		if ((!bAudioOn) && (frameNumber == 0)) bAudioOn = true;
	}
}

void autoPlayerData::draw(){

    glPushMatrix();
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, windowWidth, windowHeight, 0, -500.0, 500.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    
    ofSetColor(255, 0, 0, 255);
    
    ofPushMatrix();
    ofTranslate(0, 0, 10);
    ofNoFill();
    
    ofDrawBitmapString( "AutoPlayer", 30, 30);
    ofDrawBitmapString( title, 30, 50);
    ofDrawBitmapString( "message-> " + ofToString(message), windowWidth - 450, windowHeight - 20);
    ofDrawBitmapString( "(f)ullframe/window", 30, windowHeight - 20);
    ofDrawBitmapString( "(a)udio/soundtrack", 30, windowHeight - 40); ofDrawBitmapString(":", 180, windowHeight - 40);
    if (bAudioOn) ofDrawBitmapString("ON", 220, windowHeight - 40); else ofDrawBitmapString("OFF", 220, windowHeight - 40);
    ofDrawBitmapString( "(s)how data", 30, windowHeight - 60);
    ofDrawBitmapString( "sounds playing: " + ofToString((float)soundsPlaying, 0), 30, windowHeight - 100);
    ofDrawBitmapString( "videos playing: " + ofToString((float)videosPlaying, 0), 30, windowHeight - 120);
    ofDrawBitmapString( "fps: " + ofToString(ofGetFrameRate(), 0), windowWidth - 100, 30);
    ofDrawBitmapString( "frame number: " + ofToString(frameNumber, 0), windowWidth - 300, 30);
    ofDrawBitmapString( "seconds: " + ofToString((float)frameNumber/FRAME_RATE, 2), windowWidth - 450, 30);
    
    ofPopMatrix();
    
    ofSetColor(255);
    
    glPopMatrix();
}