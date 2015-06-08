#include "autoPlayerData.h"

autoPlayerData::autoPlayerData(){

	windowWidth = origWindowWidth = WINDOW_WIDTH_DEFAULT; windowHeight = origWindowHeight = WINDOW_HEIGHT_DEFAULT;
	bFullScreen = FULLSCREEN_DEFAULT; backgroundColor.r = backgroundColor.g = backgroundColor.b = 0;
    
    playType = LOOP;
	
	bDisplayOn = bAudioOn = true; globalAlpha = 0; bShowData = false; globalVolume = 0;
	
	frameNumber = 0; playDuration = PLAY_DURATION_MIN*FRAME_RATE;
	
	globalAlphaStep = globalVolumeStep = (float)1/(GLOBAL_TRANSITION*FRAME_RATE);
	
	videosPlaying = soundsPlaying = 0; videosPlayingMax = VIDEOS_PLAYING_MAX; soundsPlayingMax = SOUNDS_PLAYING_MAX;
	
	tiles_h = tiles_v = 1;
	
	inputType = OFF;
    
    message = "";
}

autoPlayerData::~autoPlayerData(){
	

}

void autoPlayerData::update(){
	
	frameNumber++;
	if ((frameNumber == playDuration) && (playType == LOOP)) frameNumber = 0;
	if ((frameNumber == playDuration) && (playType == ONCE)) exit(0);
	
	windowWidth = ofGetWindowWidth(); windowHeight = ofGetWindowHeight();
	
	if (bDisplayOn) globalAlpha+=globalAlphaStep; else globalAlpha-=globalAlphaStep;
	globalAlpha = ofClamp(globalAlpha, 0, 1);
	
	if (bAudioOn) globalVolume+=globalVolumeStep; else globalVolume-=globalVolumeStep;
	globalVolume = ofClamp(globalVolume, 0, 1);
	
	if (bPlayTransition){
		if ((bDisplayOn) && (frameNumber == (playDuration - (GLOBAL_TRANSITION*FRAME_RATE)))&& (playType != ENDLESS)) bDisplayOn = false;
		if ((!bDisplayOn) && (frameNumber == 0)) bDisplayOn = true;
		
		if ((bAudioOn) && (frameNumber == (playDuration - (GLOBAL_TRANSITION*FRAME_RATE)))&& (playType != ENDLESS)) bAudioOn = false;
		if ((!bAudioOn) && (frameNumber == 0)) bAudioOn = true;
	}
}