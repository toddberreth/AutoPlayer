#pragma once

#include "defines.h"
#include "ofMain.h"
#include "autoPlayerData.h"
#include <algorithm>

enum   EVENT_TYPE		{TIMED_EVENT, RANDOM_EVENT, TRIGGERED_EVENT}; 
enum   LOCATION_TYPE	{FULL, L_HALF, R_HALF, TILE, RANDOM_TILE, RANDOM_ALL}; 
enum   TYPE_OF_MEDIA	{AP_IMAGE, AP_VIDEO, AP_SOUND, AP_RANDOM_IMAGE, AP_RANDOM_VIDEO, AP_RANDOM_SOUND, AP_IMAGE_SEQUENCE, AP_VIDEO_SEQUENCE, AP_SOUND_SEQUENCE}; 

class autoPlayerMedia {
	
public:
	
	autoPlayerMedia(EVENT_TYPE eventType, autoPlayerData *data);
	~autoPlayerMedia();
	
	void setup();
	void update();
	void draw();
	
	void mouseMoved(int x, int y );
	
	EVENT_TYPE		eventType;
	LOCATION_TYPE	location;
	TYPE_OF_MEDIA	mediaType;
	
	bool			bRandomTile, bRandomAll;
	int				tile_h, tile_v;
	
	ofDirectory		mediaDirectory;
	string			mediaPath;
	
	float			alpha, alphaTarget, alphaOrig, alphaStep, alphaTransitionTime;	
	bool			bAlphaTransition;
	float			volume, volumeTarget, volumeOrig, volumeStep, volumeTransitionTime;
	bool			bVolumeTransition;
    
    bool            bRandomAlpha, bRandomSpeed, bRandomVolume, bRandomDuration;
    float           randomAlphaMin, randomAlphaMax, randomSpeedMin, randomSpeedMax;
    float           randomVolumeMin, randomVolumeMax, randomDurationMin, randomDurationMax;;
	
	float			startTime, endTime;
	int				startFrame, endFrame;
	
	bool			bPlaying, bPlayed, bKilledAlpha, bKilledVolume, bLoad, bLoaded, bUnload;
	bool			bMediaLoop;
	float			speed;
	
	float			chance;
	bool			bRepeatable;
	float			playDuration;
	int				playTimer;
	int				sequenceIndex;
	
	float			trigger_x_min, trigger_x_max, trigger_y_min, trigger_y_max;
	
	int				layer;
	
	ofVideoPlayer	video;
	ofSoundPlayer	sound;
	ofImage			image;
    
    static bool     sortGreaterLayer (autoPlayerMedia *lhs, autoPlayerMedia *rhs){return (lhs->layer > rhs->layer);}

private:

	autoPlayerData  *data;
	
};




