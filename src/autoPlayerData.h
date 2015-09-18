/*
 * AutoPlayer
 * https://github.com/toddberreth/AutoPlayer
 *
 * Copyright 2015, Todd Berreth
 *
 * Licensed under the MIT license:
 * http://www.opensource.org/licenses/MIT
 */

#pragma once

#include "defines.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "autoPlayerCams.h"

enum   PLAY_TYPE    {ONCE, LOOP, ENDLESS}; 
enum   INPUT_TYPE   {OFF, CAMERA, MOUSE};

class autoPlayerData {
	
public:
	autoPlayerData();
	virtual ~autoPlayerData();
	
	void			update();
    void			draw();
    
    void            mouseMoved(int x, int y );
    float           xPerc, yPerc;
    
    void            loadConfig();
	
	string			title;
    
	int				windowWidth, windowHeight, origWindowWidth, origWindowHeight;
	bool			bFullScreen, bDisplayOn, bAudioOn, bShowData;
	
	int				tilesH, tilesV;
	
	ofFloatColor	backgroundColor;
	
    ofxXmlSettings  config;
	string			message;
	
	float			globalAlpha, globalAlphaStep;
	float			globalVolume, globalVolumeStep;
	
	PLAY_TYPE		playType;
	int				playDurationFrames;
    float			playDurationSeconds;
    int				playLe;
	bool			bPlayTransition;
	
	INPUT_TYPE		inputType;
    
    CamData         *camData;
	
	int				frameNumber;
	
	int				videosPlaying, videosPlayingMax, soundsPlaying, soundsPlayingMax;
    
    float           randomAlphaMinTyp, randomAlphaMaxTyp, randomSpeedMinTyp, randomSpeedMaxTyp;
    float           randomVolumeMinTyp, randomVolumeMaxTyp, randomDurationMinTyp, randomDurationMaxTyp;
};