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
    
    if ((int)events.size() > 0){ for ( int i = 0; i < events.size(); i++ ) delete events[i];}
}

//--------------------------------------------------------------
void autoPlayerApp::update(){

    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {(*it)->update();}
    data->update();
    ofSoundUpdate();
    
    countVideos();
    countSounds();
    
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
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //glEnable(GL_DEPTH_TEST);
    ofPushMatrix();
    float offset = 0;
    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {(*it)->draw();}  //ofTranslate(0, 0, offset+=0.00001);
    ofPopMatrix();
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    //ofDisableAlphaBlending();
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
void autoPlayerApp::countVideos(){
    
    data->videosPlaying = 0;
    
    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {
        if ((*it)->video.isPlaying()) data->videosPlaying++;
    }
}

//--------------------------------------------------------------
void autoPlayerApp::countSounds(){
    
    data->soundsPlaying = 0;
    
    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {
        if ((*it)->sound.isPlaying()) data->soundsPlaying++;
    }
}

//--------------------------------------------------------------
void autoPlayerApp::loadConfig(){
    
    bool bLoaded = false;
    data->message = "loading config.xml";
    if( data->config.loadFile(ofFilePath::getAbsolutePath(DEFAULT_CONFIG, TRUE).c_str())){
        data->message = ofToString(DEFAULT_CONFIG) + " loaded!";
        bLoaded = true;
    }
    else{data->message = "unable to load " + ofToString(DEFAULT_CONFIG) + ", check data/ folder";}
    
    if (bLoaded){
        
        string tempString;
        //GLOBAL SETTINGS
        data->title = data->config.getValue("GLOBAL:TITLE", "AutoPlayer Project");
        data->windowWidth = data->config.getValue("GLOBAL:WINDOW:WIDTH", WINDOW_WIDTH_DEFAULT); data->origWindowWidth = data->windowWidth;
        data->windowHeight = data->config.getValue("GLOBAL:WINDOW:HEIGHT", WINDOW_HEIGHT_DEFAULT); data->origWindowHeight = data->windowHeight;
        tempString = ofToLower(data->config.getValue("GLOBAL:WINDOW:FULLSCREEN", "false"));
        if (tempString == "true") data->bFullScreen = true; else data->bFullScreen = false;
        data->tiles_h = data->config.getValue("GLOBAL:TILES:H", 1);
        data->tiles_v = data->config.getValue("GLOBAL:TILES:V", 1);
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
        if (tempString == "mouse") data->inputType = MOUSE; else if (tempString == "cameras") data->inputType = CAMERAS; else if (tempString == "off") data->inputType = OFF;
        
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
    
    string tempString;
    //newEvent->startTime = data->config.getValue("EVENT:START", 0.0, i);
    tempString = ofToLower(data->config.getValue("EVENT:START", "0", i));
    if (tempString == "start") newEvent->startTime = 0.0; else newEvent->startTime = data->config.getValue("EVENT:START", 0.0, i);
    newEvent->startTime = ofClamp(newEvent->startTime, 0.0, data->playDurationSeconds);
    newEvent->startFrame = (int)(newEvent->startTime * FRAME_RATE);
    //newEvent->endTime = data->config.getValue("EVENT:END", 0.0, i);
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
    
    newEvent->tile_h = data->config.getValue("EVENT:TILE_H", 1, i);
    newEvent->tile_v = data->config.getValue("EVENT:TILE_V", 1, i);
    
    tempString = ofToLower(data->config.getValue("EVENT:A", "1.0", i));
    if (tempString == "random") newEvent->alphaOrig = ofRandomuf(); else newEvent->alphaOrig = ofToFloat(tempString);
    tempString = ofToLower(data->config.getValue("EVENT:A_TRANS", "FALSE", i));
    if (tempString == "true") newEvent->bAlphaTransition = true; else newEvent->bAlphaTransition = false;
    newEvent->alphaTransitionTime = data->config.getValue("EVENT:A_TIME", 0.0, i);
    newEvent->volumeOrig = data->config.getValue("EVENT:V", 1, i);
    tempString = ofToLower(data->config.getValue("EVENT:V_TRANS", "FALSE", i));
    if (tempString == "true") newEvent->bVolumeTransition = true; else newEvent->bVolumeTransition = false;
    newEvent->volumeTransitionTime = data->config.getValue("EVENT:V_TIME", 0.0, i);
    tempString = ofToLower(data->config.getValue("EVENT:LOOP", "TRUE", i));
    if (tempString == "true") newEvent->mediaLoop = true; else newEvent->mediaLoop = false;
    tempString = ofToLower(data->config.getValue("EVENT:SPEED", "1.0", i));
    if (tempString == "random") newEvent->speed = ofRandomuf(); else newEvent->speed = ofToFloat(tempString);
    tempString = ofToLower(data->config.getValue("EVENT:LAYER", "1", i));
    if (tempString == "random") newEvent->layer = (rand()%9) + 1; else newEvent->layer = ofToInt(tempString);
    newEvent->chance = data->config.getValue("EVENT:CHANCE", 1.0, i);
    newEvent->playDuration = data->config.getValue("EVENT:DURATION", 0.0, i);
    tempString = ofToLower(data->config.getValue("EVENT:REPEATABLE", "TRUE", i));
    if (tempString == "true") newEvent->bRepeatable = true; else newEvent->bRepeatable = false;
    newEvent->trigger_x_min = data->config.getValue("EVENT:TRIGGER:X:MIN", 0.0, i);
    newEvent->trigger_x_max = data->config.getValue("EVENT:TRIGGER:X:MAX", 0.0, i);
    newEvent->trigger_y_min = data->config.getValue("EVENT:TRIGGER:Y:MIN", 0.0, i);
    newEvent->trigger_y_max = data->config.getValue("EVENT:TRIGGER:Y:MAX", 0.0, i);
};

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
