/*
 * AutoPlayer
 * https://github.com/toddberreth/AutoPlayer
 *
 * Copyright 2015, Todd Berreth
 *
 * Licensed under the MIT license:
 * http://www.opensource.org/licenses/MIT
 */

#include "autoPlayerMedia.h"

autoPlayerMedia::autoPlayerMedia(EVENT_TYPE	eventType, autoPlayerData *data): eventType(eventType), data(data){

	location = FULL; bRandomTile = bRandomAll = false;
	tileH = tileV = 1;
	alpha = alphaTarget = alphaOrig = alphaStep = 0;	
	volume = volumeTarget = volumeOrig = volumeStep = 0;
	startTime = endTime = startFrame = endFrame = 0;
	
	bPlaying = bKilledAlpha = bKilledVolume = bPlayed = bLoad = bUnload = bLoaded = false;
	
	alphaTransitionTime = ALPHA_TRANSITION_DEFAULT;
	volumeTransitionTime = VOLUME_TRANSITION_DEFAULT;
	
	bAlphaTransition = bVolumeTransition = false;
	
	bMediaLoop = false; speed = 1.0;
	
	playTimer = playDuration = 0;
	
	sequenceIndex = 0;
	
	layer = LAYER_DEFAULT; 
	
	trigger_x_min = trigger_x_max = trigger_y_min = trigger_y_max = 0;
    bRandomAlpha = bRandomSpeed = bRandomVolume = bRandomDuration = false;
    
    randomAlphaMin = RANDOM_ALPHA_MIN_TYP;   randomAlphaMax = RANDOM_ALPHA_MAX_TYP;
    randomSpeedMin = RANDOM_SPEED_MIN_TYP;   randomSpeedMax = RANDOM_SPEED_MAX_TYP;
    randomVolumeMin = RANDOM_VOLUME_MIN_TYP; randomVolumeMax = RANDOM_VOLUME_MAX_TYP;
    randomDurationMin = RANDOM_DURATION_MIN_TYP; randomVolumeMax = RANDOM_DURATION_MAX_TYP;
}

autoPlayerMedia::~autoPlayerMedia(){
	
    image.clear(); video.close(); sound.unload();
}

void autoPlayerMedia::setup(){
	
	startFrame = (int)(startTime * FRAME_RATE); if (startFrame <= 0) startFrame = 0; 
	if (endTime == FLT_MAX) endFrame = INT_MAX; else endFrame = (int)(endTime * FRAME_RATE); if (endFrame >= INT_MAX) endFrame = INT_MAX;
	
	if ((mediaType == AP_RANDOM_IMAGE) ||(mediaType == AP_RANDOM_VIDEO) ||(mediaType == AP_RANDOM_SOUND) ||
		(mediaType == AP_IMAGE_SEQUENCE) ||(mediaType == AP_VIDEO_SEQUENCE) ||(mediaType == AP_SOUND_SEQUENCE)
		){
		mediaDirectory.listDir(mediaPath); //mediaDirectory.sort();
	}
	
	if (eventType == TIMED_EVENT) {
		
		alphaTransitionTime = ofClamp(alphaTransitionTime, 0, (endTime - startTime)/2);
		volumeTransitionTime = ofClamp(volumeTransitionTime, 0, (endTime - startTime)/2);
	}
	
	alphaStep = 1/(alphaTransitionTime*FRAME_RATE); alphaTarget = alphaOrig;  
	volumeStep = 1/(volumeTransitionTime*FRAME_RATE); volumeTarget = volumeOrig;
	
	if (location == RANDOM_TILE){bRandomTile = true; location = TILE; tileH = rand()%data->tilesH + 1; tileV = rand()%data->tilesV + 1;}
	if (location == RANDOM_ALL) {bRandomAll = true; int tempAll = rand()%4; 
		if (tempAll == 0) location = FULL; if (tempAll == 1) location = L_HALF; if (tempAll == 2) location = R_HALF;
		if (tempAll == 3) {location = TILE;tileH = rand()%data->tilesH + 1; tileV = rand()%data->tilesV + 1;}
	}
    
    bLoaded = false;
}

void autoPlayerMedia::update(){
	
	if ((bPlaying)&&(!bKilledAlpha)) alpha+=alphaStep; else if (bKilledAlpha) alpha -= alphaStep;
	alpha = ofClamp(alpha, 0, alphaOrig);
	if ((bPlaying)&&(!bKilledVolume)) volume+=volumeStep; else if (bKilledVolume) volume -= volumeStep;
	volume = ofClamp(volume, 0, volumeOrig);
	
	if (eventType == TIMED_EVENT) {
		
		if (data->frameNumber == startFrame) {bPlaying = bPlayed = bLoad = true; bKilledAlpha = bKilledVolume = false; }
		if ((data->frameNumber == (endFrame-((int)alphaTransitionTime*FRAME_RATE))) && (bAlphaTransition)) bKilledAlpha = true;
		if ((data->frameNumber == (endFrame-((int)volumeTransitionTime*FRAME_RATE))) && (bVolumeTransition)) bKilledVolume = true;
		if (data->frameNumber == endFrame) {bPlaying = bKilledAlpha = bKilledVolume = false; bUnload = true;} //
	}
	
	if (eventType == RANDOM_EVENT) {
		
		if ((data->frameNumber >= startFrame) && (data->frameNumber <= endFrame) && (data->frameNumber%FRAME_RATE == 0) && (!bPlaying) && (bRepeatable || !bPlayed)){
			if (ofRandomuf() < chance) {bPlaying = bLoad = true; playTimer = 0;}
		}
		if (bPlaying) playTimer++;
		if ((playTimer == (((int)playDuration*FRAME_RATE)-((int)alphaTransitionTime*FRAME_RATE))) && (bAlphaTransition)) bKilledAlpha = true;
		if ((playTimer == (((int)playDuration*FRAME_RATE)-((int)volumeTransitionTime*FRAME_RATE))) && (bVolumeTransition)) bKilledVolume = true;
		if (playTimer >= ((int)playDuration*FRAME_RATE)) {bPlaying = bKilledAlpha = bKilledVolume = false; bUnload = true; playTimer = 0;} //
	}
	
	if (eventType == TRIGGERED_EVENT) {
		
        if (data->inputType == MOUSE){
            if ((data->frameNumber >= startFrame) && (data->frameNumber <= endFrame)  && (!bPlaying) && (bRepeatable || !bPlayed) && (isMouseInTriggerZone())){
                if ((chance < 1)&&((data->frameNumber%FRAME_RATE == 0)&&(ofRandomuf() < chance))) {bPlaying = bLoad = true; playTimer = 0;}
                else if (chance == 1){bPlaying = bLoad = true; playTimer = 0;}
            }
        }
		if (data->inputType == CAMERA){
            if ((data->frameNumber >= startFrame) && (data->frameNumber <= endFrame)  && (!bPlaying) && (bRepeatable || !bPlayed)&& (isBlobInTriggerZone())){
                if ((chance < 1)&&((data->frameNumber%FRAME_RATE == 0)&&(ofRandomuf() < chance))) {bPlaying = bLoad = true; playTimer = 0;}
                else if (chance == 1){bPlaying = bLoad = true; playTimer = 0;}
            }
        }
		if (bPlaying) playTimer++;
		if ((playTimer == (((int)playDuration*FRAME_RATE)-((int)alphaTransitionTime*FRAME_RATE))) && (bAlphaTransition)) bKilledAlpha = true;
		if ((playTimer == (((int)playDuration*FRAME_RATE)-((int)volumeTransitionTime*FRAME_RATE))) && (bVolumeTransition)) bKilledVolume = true;
		if (playTimer >= ((int)playDuration*FRAME_RATE)) {bPlaying = bKilledAlpha = bKilledVolume = false; bUnload = true; playTimer = 0;} //
	}
	
	if ((bLoad)&&(!bUnload)&&(!bLoaded)){
        
		if ((mediaType == AP_IMAGE)||(mediaType == AP_RANDOM_IMAGE)||(mediaType == AP_IMAGE_SEQUENCE)) {
			
            if (mediaType == AP_RANDOM_IMAGE) {mediaPath = mediaDirectory.getPath(rand()%mediaDirectory.size());}
			if (mediaType == AP_IMAGE_SEQUENCE){
				mediaPath = mediaDirectory.getPath(sequenceIndex);
				sequenceIndex++; if (sequenceIndex > (mediaDirectory.size()-1)) sequenceIndex = 0;
			}
			if (image.load(mediaPath)) {
				bLoad = false; bLoaded = true; bPlayed = true;
				if (bRandomTile) {tileH = rand()%data->tilesH + 1; tileV = rand()%data->tilesV + 1;}
				if (bRandomAll){  int tempAll = rand()%4; 
					if (tempAll == 0) location = FULL; if (tempAll == 1) location = L_HALF; if (tempAll == 2) location = R_HALF;
					if (tempAll == 3) {location = TILE;tileH = rand()%data->tilesH + 1; tileV = rand()%data->tilesV + 1;}
				}
                data->message.clear(); data->message = "image loaded -> " + mediaPath;
                
                if (bRandomAlpha) alphaOrig = ofRandom(randomAlphaMin, randomAlphaMax);
			}
            else {data->message.clear(); data->message =  "IMAGE NOT LOADED! -> " + mediaPath;}
		}
        
		if ((mediaType == AP_VIDEO)||(mediaType == AP_RANDOM_VIDEO)||(mediaType == AP_VIDEO_SEQUENCE)){

			if (mediaType == AP_RANDOM_VIDEO) mediaPath = mediaDirectory.getPath(rand()%mediaDirectory.size());
			if (mediaType == AP_VIDEO_SEQUENCE){
				mediaPath = mediaDirectory.getPath(sequenceIndex);
				sequenceIndex++; if (sequenceIndex > (mediaDirectory.size()-1)) sequenceIndex = 0;
			}
			if (data->videosPlaying < data->videosPlayingMax){
				if (video.load(mediaPath)) {

					video.setSpeed(speed);
					video.play();
                    if (bMediaLoop) video.setLoopState(OF_LOOP_NORMAL); else video.setLoopState(OF_LOOP_NONE);
					bLoad = false; bLoaded = true; bPlayed = true;
					if (bRandomTile) {tileH = rand()%data->tilesH + 1; tileV = rand()%data->tilesV + 1;}
					if (bRandomAll){  int tempAll = rand()%4; 
						if (tempAll == 0) location = FULL; if (tempAll == 1) location = L_HALF; if (tempAll == 2) location = R_HALF;
						if (tempAll == 3) {location = TILE;tileH = rand()%data->tilesH + 1; tileV = rand()%data->tilesV + 1;}
					}
                    data->message.clear(); data->message = "video loaded -> " + mediaPath;
				}
				else {data->message.clear(); data->message = "VIDEO NOT LOADED! -> " + mediaPath;}
			} else {
				data->message.clear(); data->message = "TOO MANY VIDEOS PLAYING!";
				bLoad = false; bPlaying = false; playTimer = 0;
			}
		}
        
		if ((mediaType == AP_SOUND)||(mediaType == AP_RANDOM_SOUND)||(mediaType == AP_SOUND_SEQUENCE)){
			
			if (mediaType == AP_RANDOM_SOUND) mediaPath = mediaDirectory.getPath(rand()%mediaDirectory.size());
			if (mediaType == AP_SOUND_SEQUENCE){
				mediaPath = mediaDirectory.getPath(sequenceIndex);
				sequenceIndex++; if (sequenceIndex > (mediaDirectory.size()-1)) sequenceIndex = 0;
			}
			if (data->soundsPlaying < data->soundsPlayingMax){
				if (sound.load(mediaPath)) {
                    if (bRandomVolume) volumeOrig = ofRandom(randomVolumeMin, randomVolumeMax);
                    if (bRandomSpeed) speed = ofRandom(randomSpeedMin, randomSpeedMax);
					sound.setSpeed(speed);
					sound.play();
					bLoad = false; bLoaded = true; bPlayed = true;
					sound.setMultiPlay(true); sound.setLoop(bMediaLoop);	
					if (bVolumeTransition) sound.setVolume(volume); else sound.setVolume((float)volumeOrig*data->globalVolume);
                    data->message.clear(); data->message = "sound loaded -> " + mediaPath;
				}
				else {data->message.clear(); data->message = "SOUND NOT LOADED! -> " + mediaPath;}
			} else {
				data->message.clear(); data->message = "TOO MANY SOUNDS PLAYING!";
				bLoad = false; bPlaying = false; playTimer = 0;
			}
		}
	}
	
	if ((bUnload)&&(!bLoad)&&(bLoaded)){
		
		if ((mediaType == AP_IMAGE)||(mediaType == AP_RANDOM_IMAGE)||(mediaType == AP_IMAGE_SEQUENCE)){
            image.clear(); bUnload = false; bLoaded = false;
            data->message.clear(); data->message = "image unloaded -> " + mediaPath;
        }
		if ((mediaType == AP_VIDEO)||(mediaType == AP_RANDOM_VIDEO)||(mediaType == AP_VIDEO_SEQUENCE)){
            video.stop(); video.closeMovie();bUnload = false; bLoaded = false;
            data->message.clear(); data->message = "video unloaded -> " + mediaPath;
        }
		if ((mediaType == AP_SOUND)||(mediaType == AP_RANDOM_SOUND)||(mediaType == AP_SOUND_SEQUENCE)){
            sound.unload(); bUnload = false; bLoaded = false;
            data->message.clear(); data->message = "sound unloaded -> " + mediaPath;
        }
	}
	
	if ((mediaType == AP_IMAGE)||(mediaType == AP_RANDOM_IMAGE)||(mediaType == AP_IMAGE_SEQUENCE)){}
	if ((mediaType == AP_VIDEO)||(mediaType == AP_RANDOM_VIDEO)||(mediaType == AP_VIDEO_SEQUENCE)){if (video.isLoaded()) {video.update(); video.setVolume((float)volume*data->globalVolume);}}
	if ((mediaType == AP_SOUND)||(mediaType == AP_RANDOM_SOUND)||(mediaType == AP_SOUND_SEQUENCE)){if (sound.isLoaded()) sound.setVolume((float)volume*data->globalVolume);}
}

void autoPlayerMedia::draw(){
	
	glPushMatrix();
	
	ofTranslate(0, 0,-(float)(layer-1)*0.1);
	ofSetColor(255, 255, 255, (float)(alpha*data->globalAlpha*255));
	
	if ((mediaType == AP_IMAGE)||(mediaType == AP_RANDOM_IMAGE)||(mediaType == AP_IMAGE_SEQUENCE)){
		
		if (image.isAllocated()){
			if(location == FULL) image.draw(0,0,data->windowWidth,data->windowHeight);
			if(location == L_HALF) image.draw(0,0,data->windowWidth/2,data->windowHeight);
			if(location == R_HALF) image.draw(data->windowWidth/2,0,data->windowWidth/2,data->windowHeight);
			
			if ((location == TILE) && ((tileH >=1) && (tileH <= data->tilesH)) && ((tileV >=1) && (tileV <= data->tilesV))){
				image.draw((float)(tileH-1)*(data->windowWidth/data->tilesH),(float)(tileV-1)*(data->windowHeight/data->tilesV),data->windowWidth/data->tilesH,data->windowHeight/data->tilesV);
			}
		}
	}
	
	if ((mediaType == AP_VIDEO)||(mediaType == AP_RANDOM_VIDEO)||(mediaType == AP_VIDEO_SEQUENCE)){
		
		if (video.isLoaded()){
			if(location == FULL) video.draw(0,0,data->windowWidth,data->windowHeight);
			if(location == L_HALF) video.draw(0,0,data->windowWidth/2,data->windowHeight);
			if(location == R_HALF) video.draw(data->windowWidth/2,0,data->windowWidth/2,data->windowHeight);
			if ((location == TILE) && ((tileH >=1) && (tileH <= data->tilesH)) && ((tileV >=1) && (tileV <= data->tilesV))){
				video.draw((float)(tileH-1)*(data->windowWidth/data->tilesH),(float)(tileV-1)*(data->windowHeight/data->tilesV),data->windowWidth/data->tilesH,data->windowHeight/data->tilesV);
			}
		}
	}
	
	glPopMatrix();
}

bool autoPlayerMedia::isBlobInTriggerZone(){
    
    bool inTriggerZone = false;
    
    if (data->camData->myBlobs.size() > 0){
        for(std::vector<CamBlob*>::iterator it=data->camData->myBlobs.begin(); it != data->camData->myBlobs.end(); it++){
            if (((*it)->centroidX >= trigger_x_min)&&((*it)->centroidX <= trigger_x_max)
                && ((*it)->centroidY >= trigger_y_min)&&((*it)->centroidY <= trigger_y_max)) inTriggerZone = true;
        }
    }
    
    return inTriggerZone ;
}

bool autoPlayerMedia::isMouseInTriggerZone(){
    
    bool inTriggerZone = false;
    if ((data->xPerc >= trigger_x_min)&&(data->xPerc <= trigger_x_max) && (data->yPerc >= trigger_y_min)&&(data->yPerc <= trigger_y_max)) inTriggerZone = true;
    return inTriggerZone ;
}
