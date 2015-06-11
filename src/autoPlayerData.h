#pragma once

#include "defines.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"

enum   PLAY_TYPE    {ONCE, LOOP, ENDLESS}; 
enum   INPUT_TYPE   {OFF, CAMERAS, MOUSE};

class autoPlayerData {
	
public:
	autoPlayerData();
	virtual ~autoPlayerData();
	
	void			update();
    void			draw();
    
    void            loadConfig();
	
	string			title;
    
	int				windowWidth, windowHeight, origWindowWidth, origWindowHeight;
	bool			bFullScreen, bDisplayOn, bAudioOn, bShowData;
	
	int				tiles_h, tiles_v;
	
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
	
	int				frameNumber;
	
	int				videosPlaying, videosPlayingMax, soundsPlaying, soundsPlayingMax;
    
    float           randomAlphaMinTyp, randomAlphaMaxTyp, randomSpeedMinTyp, randomSpeedMaxTyp;
    float           randomVolumeMinTyp, randomVolumeMaxTyp, randomDurationMinTyp, randomDurationMaxTyp;
};