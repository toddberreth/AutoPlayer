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
    
    if ((int)events.size() > 0){ for ( int i = 0; i < events.size(); i++ ) delete events[i]; events.clear();}
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
void autoPlayerApp::keyReleased(int key){

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
        
        //cout << numTimedEvents << endl;
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
        
        
        sort(events.begin(),events.end(), autoPlayerMedia::sortGreaterLayer);
    }
}



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
    newEvent->chance = data->config.getValue("EVENT:CHANCE", 0.0, i);
    newEvent->playDuration = data->config.getValue("EVENT:DURATION", 0.0, i);
    tempString = ofToLower(data->config.getValue("EVENT:REPEATABLE", "TRUE", i));
    if (tempString == "true") newEvent->bRepeatable = true; else newEvent->bRepeatable = false;
};

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


/*
 data->message = "loading config.xml";
 if( data->config.LoadFile(ofFilePath::getAbsolutePath(DEFAULT_CONFIG, TRUE).c_str()) ){data->message = "config.xml loaded!";}
 else{data->message = "unable to load config.xml check data/ folder";}
 
 TiXmlNode*		node;
 TiXmlElement*	element;
 TiXmlNode*		eventNode;
 TiXmlElement*	eventElement;
 
 for(TiXmlNode* node = data->config.FirstChild(); node; node = node->NextSibling() ){
 
 for( element = node->FirstChildElement(); element; element = element->NextSiblingElement() ){
 
 
 
 if (node->ValueStr()=="TimedEvent") {
 
 if (element->ValueStr()=="Timed") {
 autoPlayerMedia * newTimedEvent = new autoPlayerMedia(TIMED_EVENT, data);
 string tempString;
 element->QueryStringAttribute("start", &tempString); tempString = ofToLower(tempString);
 if (tempString == "start") newTimedEvent->startTime = 0;
 else {element->QueryFloatAttribute("start", &newTimedEvent->startTime);}
 element->QueryStringAttribute("end", &tempString); tempString = ofToLower(tempString);
 if (tempString == "end") newTimedEvent->endTime = FLT_MAX;
 else {element->QueryFloatAttribute("end", & newTimedEvent->endTime);}
 element->QueryStringAttribute("type", &tempString); tempString = ofToLower(tempString);
 if (tempString == "image") newTimedEvent->mediaType = AP_IMAGE; if (tempString == "video") newTimedEvent->mediaType = AP_VIDEO; if (tempString == "sound") newTimedEvent->mediaType = AP_SOUND;
 if (tempString == "random_image") newTimedEvent->mediaType = AP_RANDOM_IMAGE; if (tempString == "random_video") newTimedEvent->mediaType = AP_RANDOM_VIDEO; if (tempString == "random_sound") newTimedEvent->mediaType = AP_RANDOM_SOUND;
 if (tempString == "image_sequence") newTimedEvent->mediaType = AP_IMAGE_SEQUENCE; if (tempString == "video_sequence") newTimedEvent->mediaType = AP_VIDEO_SEQUENCE; if (tempString == "sound_sequence") newTimedEvent->mediaType = AP_SOUND_SEQUENCE;
 element->QueryStringAttribute("location", &tempString); tempString = ofToLower(tempString);
 if (tempString == "full") newTimedEvent->location = FULL; if (tempString == "l_half") newTimedEvent->location = L_HALF; if (tempString == "r_half") newTimedEvent->location = R_HALF;
 if (tempString == "tile") newTimedEvent->location = TILE; if (tempString == "random_tile") newTimedEvent->location = RANDOM_TILE; if (tempString == "random") newTimedEvent->location = RANDOM_ALL;
 element->QueryIntAttribute("tile_h", &newTimedEvent->tile_h);
 element->QueryIntAttribute("tile_v", &newTimedEvent->tile_v);
 element->QueryStringAttribute("path", &newTimedEvent->mediaPath);
 element->QueryFloatAttribute("alpha", &newTimedEvent->alphaOrig);
 element->QueryStringAttribute("alpha_transition", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newTimedEvent->bAlphaTransition = true;} else {newTimedEvent->bAlphaTransition = false;}
 element->QueryFloatAttribute("alpha_transition_time", &newTimedEvent->alphaTransitionTime);
 element->QueryFloatAttribute("volume", &newTimedEvent->volumeOrig);
 element->QueryStringAttribute("volume_transition", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newTimedEvent->bVolumeTransition = true;} else {newTimedEvent->bVolumeTransition = false;}
 element->QueryFloatAttribute("volume_transition_time", &newTimedEvent->volumeTransitionTime);
 element->QueryStringAttribute("loop", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newTimedEvent->mediaLoop = true;} else {newTimedEvent->mediaLoop = false;}
 element->QueryFloatAttribute("speed", &newTimedEvent->speed);
 element->QueryIntAttribute("layer", &newTimedEvent->layer);
 newTimedEvent->setup();
 events.push_back(newTimedEvent);
 }
 }
 
 
 if (node->ValueStr()=="RandomEvent") {
 
 if (element->ValueStr()=="Random") {
 autoPlayerMedia * newRandomEvent = new autoPlayerMedia(RANDOM_EVENT, data);
 string tempString;
 element->QueryStringAttribute("start", &tempString); tempString = ofToLower(tempString);
 if (tempString == "start") newRandomEvent->startTime = 0;
 else {element->QueryFloatAttribute("start", &newRandomEvent->startTime);}
 element->QueryStringAttribute("end", &tempString); tempString = ofToLower(tempString);
 if (tempString == "end") newRandomEvent->endTime = FLT_MAX;
 else {element->QueryFloatAttribute("end", & newRandomEvent->endTime);}
 element->QueryStringAttribute("type", &tempString); tempString = ofToLower(tempString);
 if (tempString == "image") newRandomEvent->mediaType = AP_IMAGE; if (tempString == "video") newRandomEvent->mediaType = AP_VIDEO; if (tempString == "sound") newRandomEvent->mediaType = AP_SOUND;
 if (tempString == "random_image") newRandomEvent->mediaType = AP_RANDOM_IMAGE; if (tempString == "random_video") newRandomEvent->mediaType = AP_RANDOM_VIDEO; if (tempString == "random_sound") newRandomEvent->mediaType = AP_RANDOM_SOUND;
 if (tempString == "image_sequence") newRandomEvent->mediaType = AP_IMAGE_SEQUENCE; if (tempString == "video_sequence") newRandomEvent->mediaType = AP_VIDEO_SEQUENCE; if (tempString == "sound_sequence") newRandomEvent->mediaType = AP_SOUND_SEQUENCE;
 element->QueryStringAttribute("location", &tempString); tempString = ofToLower(tempString);
 if (tempString == "full") newRandomEvent->location = FULL; if (tempString == "l_half") newRandomEvent->location = L_HALF; if (tempString == "r_half") newRandomEvent->location = R_HALF;
 if (tempString == "tile") newRandomEvent->location = TILE; if (tempString == "random_tile") newRandomEvent->location = RANDOM_TILE; if (tempString == "random") newRandomEvent->location = RANDOM_ALL;
 element->QueryIntAttribute("tile_h", &newRandomEvent->tile_h);
 element->QueryIntAttribute("tile_v", &newRandomEvent->tile_v);
 element->QueryStringAttribute("path", &newRandomEvent->mediaPath);
 element->QueryFloatAttribute("alpha", &newRandomEvent->alphaOrig);
 element->QueryStringAttribute("alpha_transition", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newRandomEvent->bAlphaTransition = true;} else {newRandomEvent->bAlphaTransition = false;}
 element->QueryFloatAttribute("alpha_transition_time", &newRandomEvent->alphaTransitionTime);
 element->QueryFloatAttribute("volume", &newRandomEvent->volumeOrig);
 element->QueryStringAttribute("volume_transition", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newRandomEvent->bVolumeTransition = true;} else {newRandomEvent->bVolumeTransition = false;}
 element->QueryFloatAttribute("volume_transition_time", &newRandomEvent->volumeTransitionTime);
 element->QueryStringAttribute("loop", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newRandomEvent->mediaLoop = true;} else {newRandomEvent->mediaLoop = false;}
 element->QueryFloatAttribute("speed", &newRandomEvent->speed);
 element->QueryIntAttribute("layer", &newRandomEvent->layer);
 element->QueryFloatAttribute("chance", &newRandomEvent->chance);
 element->QueryFloatAttribute("duration", &newRandomEvent->playDuration);
 element->QueryStringAttribute("repeatable", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newRandomEvent->bRepeatable = true;} else {newRandomEvent->bRepeatable = false;}
 
 newRandomEvent->setup();
 events.push_back(newRandomEvent);
 }
 
 }
 
 
 if (node->ValueStr()=="TriggeredEvent") {
 
 if (element->ValueStr()=="Trigger") {
 autoPlayerMedia * newTriggeredEvent = new autoPlayerMedia(TRIGGERED_EVENT, data);
 string tempString;
 element->QueryStringAttribute("start", &tempString); tempString = ofToLower(tempString);
 if (tempString == "start") newTriggeredEvent->startTime = 0;
 else {element->QueryFloatAttribute("start", &newTriggeredEvent->startTime);}
 element->QueryStringAttribute("end", &tempString); tempString = ofToLower(tempString);
 if (tempString == "end") newTriggeredEvent->endTime = FLT_MAX;
 else {element->QueryFloatAttribute("end", & newTriggeredEvent->endTime);}
 element->QueryStringAttribute("type", &tempString); tempString = ofToLower(tempString);
 if (tempString == "image") newTriggeredEvent->mediaType = AP_IMAGE; if (tempString == "video") newTriggeredEvent->mediaType = AP_VIDEO; if (tempString == "sound") newTriggeredEvent->mediaType = AP_SOUND;
 if (tempString == "random_image") newTriggeredEvent->mediaType = AP_RANDOM_IMAGE; if (tempString == "random_video") newTriggeredEvent->mediaType = AP_RANDOM_VIDEO; if (tempString == "random_sound") newTriggeredEvent->mediaType = AP_RANDOM_SOUND;
 if (tempString == "image_sequence") newTriggeredEvent->mediaType = AP_IMAGE_SEQUENCE; if (tempString == "video_sequence") newTriggeredEvent->mediaType = AP_VIDEO_SEQUENCE; if (tempString == "sound_sequence") newTriggeredEvent->mediaType = AP_SOUND_SEQUENCE;
 element->QueryStringAttribute("location", &tempString); tempString = ofToLower(tempString);
 if (tempString == "full") newTriggeredEvent->location = FULL; if (tempString == "l_half") newTriggeredEvent->location = L_HALF; if (tempString == "r_half") newTriggeredEvent->location = R_HALF;
 if (tempString == "tile") newTriggeredEvent->location = TILE; if (tempString == "random_tile") newTriggeredEvent->location = RANDOM_TILE; if (tempString == "random") newTriggeredEvent->location = RANDOM_ALL;
 element->QueryIntAttribute("tile_h", &newTriggeredEvent->tile_h);
 element->QueryIntAttribute("tile_v", &newTriggeredEvent->tile_v);
 element->QueryStringAttribute("path", &newTriggeredEvent->mediaPath);
 element->QueryFloatAttribute("alpha", &newTriggeredEvent->alphaOrig);
 element->QueryStringAttribute("alpha_transition", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newTriggeredEvent->bAlphaTransition = true;} else {newTriggeredEvent->bAlphaTransition = false;}
 element->QueryFloatAttribute("alpha_transition_time", &newTriggeredEvent->alphaTransitionTime);
 element->QueryFloatAttribute("volume", &newTriggeredEvent->volumeOrig);
 element->QueryStringAttribute("volume_transition", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newTriggeredEvent->bVolumeTransition = true;} else {newTriggeredEvent->bVolumeTransition = false;}
 element->QueryFloatAttribute("volume_transition_time", &newTriggeredEvent->volumeTransitionTime);
 element->QueryStringAttribute("loop", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newTriggeredEvent->mediaLoop = true;} else {newTriggeredEvent->mediaLoop = false;}
 element->QueryFloatAttribute("speed", &newTriggeredEvent->speed);
 element->QueryIntAttribute("layer", &newTriggeredEvent->layer);
 element->QueryFloatAttribute("chance", &newTriggeredEvent->chance);
 element->QueryFloatAttribute("duration", &newTriggeredEvent->playDuration);
 element->QueryStringAttribute("repeatable", &tempString); tempString = ofToLower(tempString);
 if (tempString == "true") {newTriggeredEvent->bRepeatable = true;} else {newTriggeredEvent->bRepeatable = false;}
 
 element->QueryFloatAttribute("trigger_x_min", &newTriggeredEvent->trigger_x_min);
 element->QueryFloatAttribute("trigger_x_max", &newTriggeredEvent->trigger_x_max);
 element->QueryFloatAttribute("trigger_y_min", &newTriggeredEvent->trigger_y_min);
 element->QueryFloatAttribute("trigger_y_max", &newTriggeredEvent->trigger_y_max);
 
 newTriggeredEvent->setup();
 events.push_back(newTriggeredEvent);
 }
 }
 
 }
 }
 */
