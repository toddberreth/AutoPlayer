#include "autoPlayerMedia.h"


autoPlayerMedia::autoPlayerMedia(EVENT_TYPE	eventType, autoPlayerData *data): eventType(eventType), data(data){

	location = FULL; bRandomTile = bRandomAll = false;
	tile_h = tile_v = 1;
	alpha = alphaTarget = alphaOrig = alphaStep = 0;	
	volume = volumeTarget = volumeOrig = volumeStep = 0;
	startTime = endTime = startFrame = endFrame = 0;
	
	bPlaying = bKilledAlpha = bKilledVolume = bPlayed = bLoad = bUnload = bLoaded = false;
	
	alphaTransitionTime = ALPHA_TRANSITION_DEFAULT;
	volumeTransitionTime = VOLUME_TRANSITION_DEFAULT;
	
	bAlphaTransition = bVolumeTransition = false;
	
	mediaLoop = false; speed = 1.0;
	
	playTimer = playDuration = 0;
	
	sequenceIndex = 0;
	
	layer = LAYER_DEFAULT; 
	
	trigger_x_min = trigger_x_max = trigger_y_min = trigger_y_max = 0;
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
	
	if (location == RANDOM_TILE){bRandomTile = true; location = TILE; tile_h = rand()%data->tiles_h + 1; tile_v = rand()%data->tiles_v + 1;}
	if (location == RANDOM_ALL) {bRandomAll = true; int tempAll = rand()%4; 
		if (tempAll == 0) location = FULL; if (tempAll == 1) location = L_HALF; if (tempAll == 2) location = R_HALF;
		if (tempAll == 3) {location = TILE;tile_h = rand()%data->tiles_h + 1; tile_v = rand()%data->tiles_v + 1;}
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
		
		if (data->inputType == CAMERAS){}
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
				if (bRandomTile) {tile_h = rand()%data->tiles_h + 1; tile_v = rand()%data->tiles_v + 1;}
				if (bRandomAll){  int tempAll = rand()%4; 
					if (tempAll == 0) location = FULL; if (tempAll == 1) location = L_HALF; if (tempAll == 2) location = R_HALF;
					if (tempAll == 3) {location = TILE;tile_h = rand()%data->tiles_h + 1; tile_v = rand()%data->tiles_v + 1;}
				}
                data->message.clear(); data->message = mediaPath + " -- image loaded";
			}
            else {data->message.clear(); data->message = mediaPath + " -- IMAGE NOT LOADED!";}  //cout << mediaPath.c_str() << " -- image not loaded" << endl;}
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
                    if (mediaLoop) video.setLoopState(OF_LOOP_NORMAL); else video.setLoopState(OF_LOOP_NONE);
					bLoad = false; bLoaded = true; bPlayed = true;
					if (bRandomTile) {tile_h = rand()%data->tiles_h + 1; tile_v = rand()%data->tiles_v + 1;}
					if (bRandomAll){  int tempAll = rand()%4; 
						if (tempAll == 0) location = FULL; if (tempAll == 1) location = L_HALF; if (tempAll == 2) location = R_HALF;
						if (tempAll == 3) {location = TILE;tile_h = rand()%data->tiles_h + 1; tile_v = rand()%data->tiles_v + 1;}
					}
                    data->message.clear(); data->message = mediaPath + " -- video loaded";
				}
				else {data->message.clear(); data->message = mediaPath + " -- VIDEO NOT LOADED!";}
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
					sound.setSpeed(speed);
					sound.play();
					bLoad = false; bLoaded = true; bPlayed = true;
					sound.setMultiPlay(true); sound.setLoop(mediaLoop);	
					if (bVolumeTransition) sound.setVolume(volume); else sound.setVolume((float)volumeOrig*data->globalVolume);
                    data->message.clear(); data->message = mediaPath + " -- sound loaded";
				}
				else {data->message.clear(); data->message = mediaPath + " -- SOUND NOT LOADED!";}
			} else {
				data->message.clear(); data->message = "TOO MANY SOUNDS PLAYING!";
				bLoad = false; bPlaying = false; playTimer = 0;
			}
		}
	}
	
	if ((bUnload)&&(!bLoad)&&(bLoaded)){
		
		if ((mediaType == AP_IMAGE)||(mediaType == AP_RANDOM_IMAGE)||(mediaType == AP_IMAGE_SEQUENCE)){
            image.clear(); bUnload = false; bLoaded = false;
            data->message.clear(); data->message = mediaPath + " -- image unloaded";
        }
		if ((mediaType == AP_VIDEO)||(mediaType == AP_RANDOM_VIDEO)||(mediaType == AP_VIDEO_SEQUENCE)){
            video.stop(); video.closeMovie();bUnload = false; bLoaded = false;
            data->message.clear(); data->message = mediaPath + " -- video unloaded";
        }
		if ((mediaType == AP_SOUND)||(mediaType == AP_RANDOM_SOUND)||(mediaType == AP_SOUND_SEQUENCE)){
            sound.unload(); bUnload = false; bLoaded = false;
            data->message.clear(); data->message = mediaPath + " -- sound unloaded";
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
			
			if ((location == TILE) && ((tile_h >=1) && (tile_h <= data->tiles_h)) && ((tile_v >=1) && (tile_v <= data->tiles_v))){
				image.draw((float)(tile_h-1)*(data->windowWidth/data->tiles_h),(float)(tile_v-1)*(data->windowHeight/data->tiles_v),data->windowWidth/data->tiles_h,data->windowHeight/data->tiles_v);
			}
		}
	}
	
	if ((mediaType == AP_VIDEO)||(mediaType == AP_RANDOM_VIDEO)||(mediaType == AP_VIDEO_SEQUENCE)){
		
		if (video.isLoaded()){
			if(location == FULL) video.draw(0,0,data->windowWidth,data->windowHeight);
			if(location == L_HALF) video.draw(0,0,data->windowWidth/2,data->windowHeight);
			if(location == R_HALF) video.draw(data->windowWidth/2,0,data->windowWidth/2,data->windowHeight);
			if ((location == TILE) && ((tile_h >=1) && (tile_h <= data->tiles_h)) && ((tile_v >=1) && (tile_v <= data->tiles_v))){
				video.draw((float)(tile_h-1)*(data->windowWidth/data->tiles_h),(float)(tile_v-1)*(data->windowHeight/data->tiles_v),data->windowWidth/data->tiles_h,data->windowHeight/data->tiles_v);
			}
		}
	}
	
	glPopMatrix();
}

//--------------------------------------------------------------
void autoPlayerMedia::mouseMoved(int x, int y){
	
	float xPerc, yPerc; xPerc = (float)x/data->windowWidth; yPerc = (float)y/data->windowHeight; 

	if (data->inputType == MOUSE){
		
		if ((data->frameNumber >= startFrame) && (data->frameNumber <= endFrame) && (data->frameNumber%FRAME_RATE == 0) && (!bPlaying) && (bRepeatable || !bPlayed)
			&& ((xPerc >= trigger_x_min)&&(xPerc <= trigger_x_max)) && ((yPerc >= trigger_y_min)&&(yPerc <= trigger_y_max))){
			if (ofRandomuf() < chance) {bPlaying = bPlayed = bLoad = true; playTimer = 0;}
		}
	}
}