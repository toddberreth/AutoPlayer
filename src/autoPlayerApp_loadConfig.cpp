/*
 * AutoPlayer
 * https://github.com/toddberreth/AutoPlayer
 *
 * Copyright 2015, Todd Berreth
 *
 * Licensed under the MIT license:
 * http://www.opensource.org/licenses/MIT
 */

#include "autoPlayerApp.h"

//--------------------------------------------------------------
void autoPlayerApp::loadConfig(){
    
    ofxXmlSettings  initConfig;
    initConfig.loadFile(DEFAULT_LOAD);
    string configXML = initConfig.getValue("LOAD_CONFIG", "");
    
    bool bLoaded = false;
    data->message = "loading " + configXML;
    if( data->config.loadFile(configXML)){data->message = configXML + " loaded!"; bLoaded = true;}
    else{data->message = "unable to load " + configXML + ", check data/ folder";}
    
    if (bLoaded){
        
        string tempString;
        //GLOBAL SETTINGS
        data->title = data->config.getValue("GLOBAL:TITLE", "AutoPlayer Project");
        data->windowWidth = data->config.getValue("GLOBAL:WINDOW:WIDTH", WINDOW_WIDTH_DEFAULT); data->origWindowWidth = data->windowWidth;
        data->windowHeight = data->config.getValue("GLOBAL:WINDOW:HEIGHT", WINDOW_HEIGHT_DEFAULT); data->origWindowHeight = data->windowHeight;
        tempString = ofToLower(data->config.getValue("GLOBAL:WINDOW:FULLSCREEN", "false"));
        if (tempString == "true") data->bFullScreen = true; else data->bFullScreen = false;
        data->tilesH = data->config.getValue("GLOBAL:TILES:H", 1);
        data->tilesV = data->config.getValue("GLOBAL:TILES:V", 1);
        data->backgroundColor.r = data->config.getValue("GLOBAL:BKG_COLOR:R", 0);
        data->backgroundColor.g = data->config.getValue("GLOBAL:BKG_COLOR:G", 0);
        data->backgroundColor.b = data->config.getValue("GLOBAL:BKG_COLOR:B", 0);
        tempString = ofToLower(data->config.getValue("GLOBAL:PLAYING:TYPE", "once"));
        if (tempString == "loop") data->playType = LOOP; else if (tempString == "endless") data->playType = ENDLESS; else data->playType = ONCE;
        data->playDurationSeconds = data->config.getValue("GLOBAL:PLAYING:DURATION", 0.0); data->playDurationFrames = (int)(data->playDurationSeconds * FRAME_RATE);
        tempString = ofToLower(data->config.getValue("GLOBAL:PLAYING:TRANSITION", "false"));
        if (tempString == "true") {data->bPlayTransition = true; data->globalAlpha = 0;} else {data->bPlayTransition = false; data->globalAlpha = 1;}
        data->videosPlayingMax = data->config.getValue("GLOBAL:VIDEOS_MAX", VIDEOS_PLAYING_MAX_DEFAULT);
        data->soundsPlayingMax = data->config.getValue("GLOBAL:SOUNDS_MAX", SOUNDS_PLAYING_MAX_DEFAULT);
        tempString = ofToLower(data->config.getValue("GLOBAL:SHOW_DATA", "false"));
        if (tempString == "true") data->bShowData = true; else data->bShowData = false;
        tempString = ofToLower(data->config.getValue("GLOBAL:INPUT_TYPE", "off"));
        if (tempString == "mouse") data->inputType = MOUSE; else if (tempString == "camera") data->inputType = CAMERA; else if (tempString == "off") data->inputType = OFF;
        data->randomAlphaMinTyp     = data->config.getValue("GLOBAL:RANDOM_ALPHA_TYP:MIN", RANDOM_ALPHA_MIN_TYP);
        data->randomAlphaMaxTyp     = data->config.getValue("GLOBAL:RANDOM_ALPHA_TYP:MAX", RANDOM_ALPHA_MAX_TYP);
        data->randomSpeedMinTyp     = data->config.getValue("GLOBAL:RANDOM_SPEED_TYP:MIN", RANDOM_SPEED_MIN_TYP);
        data->randomSpeedMaxTyp     = data->config.getValue("GLOBAL:RANDOM_SPEED_TYP:MAX", RANDOM_SPEED_MAX_TYP);
        data->randomVolumeMinTyp    = data->config.getValue("GLOBAL:RANDOM_VOLUME_TYP:MIN", RANDOM_VOLUME_MIN_TYP);
        data->randomVolumeMaxTyp    = data->config.getValue("GLOBAL:RANDOM_VOLUME_TYP:MAX", RANDOM_VOLUME_MAX_TYP);
        data->randomDurationMinTyp  = data->config.getValue("GLOBAL:RANDOM_DURATION_TYP:MIN", RANDOM_DURATION_MIN_TYP);
        data->randomDurationMaxTyp  = data->config.getValue("GLOBAL:RANDOM_DURATION_TYP:MAX", RANDOM_DURATION_MAX_TYP);
        
        //TIMED EVENTS
        data->config.pushTag("TIMED_EVENTS", 0);
        int numTimedEvents = data->config.getNumTags("EVENT");
        for(int i = 0; i < numTimedEvents; i++){
            autoPlayerMedia * newTimedEvent = new autoPlayerMedia(TIMED_EVENT, data);
            loadEvent(newTimedEvent, i);
            newTimedEvent->setup();
            events.push_back(newTimedEvent);
        }
        data->config.popTag();
        
        //RANDOM EVENTS
        data->config.pushTag("RANDOM_EVENTS", 0);
        int numRandomEvents = data->config.getNumTags("EVENT");
        for(int i = 0; i < numRandomEvents; i++){
            autoPlayerMedia * newRandomEvent = new autoPlayerMedia(RANDOM_EVENT, data);
            loadEvent(newRandomEvent, i);
            newRandomEvent->setup();
            events.push_back(newRandomEvent);
        }
        data->config.popTag();
        
        //TRIGGERED EVENTS
        data->config.pushTag("TRIGGERED_EVENTS", 0);
        int numTriggeredEvents = data->config.getNumTags("EVENT");
        for(int i = 0; i < numTriggeredEvents; i++){
            autoPlayerMedia * newTriggeredEvent = new autoPlayerMedia(TRIGGERED_EVENT, data);
            loadEvent(newTriggeredEvent, i);
            newTriggeredEvent->setup();
            events.push_back(newTriggeredEvent);
        }
        data->config.popTag();
        
        //sorts events by layer
        sort(events.begin(),events.end(), autoPlayerMedia::sortGreaterLayer);
    }
}

//--------------------------------------------------------------
void autoPlayerApp::loadEvent(autoPlayerMedia *newEvent, int i){
    
    string tempString; float tempFloat;
    tempString = ofToLower(data->config.getValue("EVENT:START", "0", i));
    if (tempString == "start") newEvent->startTime = 0.0; else newEvent->startTime = data->config.getValue("EVENT:START", 0.0, i);
    newEvent->startTime = ofClamp(newEvent->startTime, 0.0, data->playDurationSeconds);
    newEvent->startFrame = (int)(newEvent->startTime * FRAME_RATE);
    tempString = ofToLower(data->config.getValue("EVENT:END", "0", i));
    if (tempString == "end"){if (data->playType == ENDLESS) newEvent->endTime = FLT_MAX; else newEvent->endTime = data->playDurationSeconds;}
    else newEvent->endTime = data->config.getValue("EVENT:END", 0.0, i);
    if (data->playType != ENDLESS) newEvent->endTime = ofClamp(newEvent->endTime, newEvent->startTime, data->playDurationSeconds);
    newEvent->endFrame = (int)(newEvent->endTime * FRAME_RATE);
    
    tempString = ofToLower(data->config.getValue("EVENT:TYPE", "", i));
    if (tempString == "image") newEvent->mediaType = AP_IMAGE;
    if (tempString == "video") newEvent->mediaType = AP_VIDEO;
    if (tempString == "sound") newEvent->mediaType = AP_SOUND;
    if (tempString == "randomimage") newEvent->mediaType = AP_RANDOM_IMAGE;
    if (tempString == "randomvideo") newEvent->mediaType = AP_RANDOM_VIDEO;
    if (tempString == "randomsound") newEvent->mediaType = AP_RANDOM_SOUND;
    if (tempString == "imagesequence") newEvent->mediaType = AP_IMAGE_SEQUENCE;
    if (tempString == "videosequence") newEvent->mediaType = AP_VIDEO_SEQUENCE;
    if (tempString == "soundsequence") newEvent->mediaType = AP_SOUND_SEQUENCE;
    
    newEvent->mediaPath = data->config.getValue("EVENT:PATH", "", i);
    tempString = ofToLower(data->config.getValue("EVENT:LOC", "", i));
    if (tempString == "full") newEvent->location = FULL;
    if (tempString == "lhalf") newEvent->location = L_HALF;
    if (tempString == "rhalf") newEvent->location = R_HALF;
    if (tempString == "tile") newEvent->location = TILE;
    if (tempString == "randomtile") newEvent->location = RANDOM_TILE;
    if (tempString == "random") newEvent->location = RANDOM_ALL;
    
    newEvent->tileH = data->config.getValue("EVENT:TILE_H", 1, i);
    newEvent->tileV = data->config.getValue("EVENT:TILE_V", 1, i);
    
    //set alpha values
    tempString = ofToLower(data->config.getValue("EVENT:A", "1.0", i));
    if (tempString == "random"){
        newEvent->bRandomAlpha = true;
        newEvent->randomAlphaMin = data->randomAlphaMinTyp; newEvent->randomAlphaMax = data->randomAlphaMaxTyp;
        tempFloat = data->config.getValue("EVENT:A_RND_MIN", -1.0, i); if ((tempFloat >= 0) && (tempFloat <= 1)) newEvent->randomAlphaMin = tempFloat;
        tempFloat = data->config.getValue("EVENT:A_RND_MAX", -1.0, i); if ((tempFloat >= 0) && (tempFloat <= 1)) newEvent->randomAlphaMax = tempFloat;
        newEvent->alphaOrig = ofRandom(newEvent->randomAlphaMin, newEvent->randomAlphaMax);
    }
    else newEvent->alphaOrig = ofToFloat(tempString);
    tempString = ofToLower(data->config.getValue("EVENT:A_TRANS", "FALSE", i));
    if (tempString == "true") newEvent->bAlphaTransition = true; else newEvent->bAlphaTransition = false;
    newEvent->alphaTransitionTime = data->config.getValue("EVENT:A_TIME", 0.0, i);
    
    //set volume values
    tempString = ofToLower(data->config.getValue("EVENT:V", "1.0", i));
    if (tempString == "random"){
        newEvent->bRandomVolume = true;
        newEvent->randomVolumeMin = data->randomVolumeMinTyp; newEvent->randomVolumeMax = data->randomVolumeMaxTyp;
        tempFloat = data->config.getValue("EVENT:V_RND_MIN", -1.0, i); if ((tempFloat >= 0) && (tempFloat <= 1)) newEvent->randomVolumeMin = tempFloat;
        tempFloat = data->config.getValue("EVENT:V_RND_MAX", -1.0, i); if ((tempFloat >= 0) && (tempFloat <= 1)) newEvent->randomVolumeMax = tempFloat;
        newEvent->volumeOrig = ofRandom(newEvent->randomVolumeMin, newEvent->randomVolumeMax);
    }
    else newEvent->volumeOrig = ofToFloat(tempString);
    tempString = ofToLower(data->config.getValue("EVENT:V_TRANS", "FALSE", i));
    if (tempString == "true") newEvent->bVolumeTransition = true; else newEvent->bVolumeTransition = false;
    newEvent->volumeTransitionTime = data->config.getValue("EVENT:V_TIME", 0.0, i);
    
    //set speed values
    tempString = ofToLower(data->config.getValue("EVENT:SPEED", "1.0", i));
    if (tempString == "random"){
        newEvent->bRandomSpeed = true;
        newEvent->randomSpeedMin = data->randomSpeedMinTyp; newEvent->randomSpeedMax = data->randomSpeedMaxTyp;
        tempFloat = data->config.getValue("EVENT:S_RND_MIN", -1.0, i); if ((tempFloat >= 0.0) && (tempFloat <= 2)) newEvent->randomSpeedMin = tempFloat;
        tempFloat = data->config.getValue("EVENT:S_RND_MAX", -1.0, i); if ((tempFloat >= 0.0) && (tempFloat <= 2)) newEvent->randomSpeedMax = tempFloat;
        newEvent->speed = ofRandom(newEvent->randomSpeedMin, newEvent->randomSpeedMax);
    }
    else newEvent->speed = ofToFloat(tempString);
    
    tempString = ofToLower(data->config.getValue("EVENT:LOOP", "TRUE", i));
    if (tempString == "true") newEvent->bMediaLoop = true; else newEvent->bMediaLoop = false;
    tempString = ofToLower(data->config.getValue("EVENT:LAYER", "1", i));
    if (tempString == "random") newEvent->layer = (rand()%9) + 1; else newEvent->layer = ofToInt(tempString);
    newEvent->chance = data->config.getValue("EVENT:CHANCE", 1.0, i);
    
    tempString = ofToLower(data->config.getValue("EVENT:REPEATABLE", "TRUE", i));
    if (tempString == "true") newEvent->bRepeatable = true; else newEvent->bRepeatable = false;
    newEvent->trigger_x_min = data->config.getValue("EVENT:TRIGGER:X:MIN", 0.0, i);
    newEvent->trigger_x_max = data->config.getValue("EVENT:TRIGGER:X:MAX", 0.0, i);
    newEvent->trigger_y_min = data->config.getValue("EVENT:TRIGGER:Y:MIN", 0.0, i);
    newEvent->trigger_y_max = data->config.getValue("EVENT:TRIGGER:Y:MAX", 0.0, i);
    
    tempString = ofToLower(data->config.getValue("EVENT:DURATION", "0.0", i));
    if (tempString == "random"){
        newEvent->bRandomDuration = true;
        newEvent->randomDurationMin = data->randomDurationMinTyp; newEvent->randomDurationMax = data->randomDurationMaxTyp;
        tempFloat = data->config.getValue("EVENT:D_RND_MIN", -1.0, i); if ((tempFloat >= 0.0) && (tempFloat <= FLT_MAX)) newEvent->randomDurationMin = tempFloat;
        tempFloat = data->config.getValue("EVENT:D_RND_MAX", -1.0, i); if ((tempFloat >= 0.0) && (tempFloat <= FLT_MAX)) newEvent->randomDurationMax = tempFloat;
        newEvent->playDuration = ofRandom(newEvent->randomDurationMin, newEvent->randomDurationMax);
    }
    else newEvent->playDuration = ofToFloat(tempString);
};
