#include "autoPlayerApp.h"



//--------------------------------------------------------------
void autoPlayerApp::setup(){
    
    data = new autoPlayerData();
    
    ofSetFrameRate(FRAME_RATE);
    ofSetVerticalSync(VERTICAL_SYNC);
    
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
    ofPushMatrix();
    float offset = 0;
    for (vector<autoPlayerMedia*>::iterator it = events.begin(); it != events.end(); it++) {(*it)->draw(); ofTranslate(0, 0, offset+=0.00001);}
    ofPopMatrix();
    glDisable(GL_BLEND);
    if (data->bShowData) drawData();
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
void autoPlayerApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void autoPlayerApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void autoPlayerApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void autoPlayerApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void autoPlayerApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void autoPlayerApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void autoPlayerApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void autoPlayerApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void autoPlayerApp::drawData(){
    
    glPushMatrix();
    
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, data->windowWidth, data->windowHeight, 0, -500.0, 500.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    
    ofSetColor(255, 0, 0, 255);
    
    ofPushMatrix();
    ofTranslate(0, 0, 10);
    ofNoFill();
    
    ofDrawBitmapString( data->title, 30, 25);
    
    ofDrawBitmapString( "AUTOPLAYER", 30, 30);
    ofDrawBitmapString( "message-> " + ofToString(data->message), data->windowWidth - 350, data->windowHeight - 20);
    ofDrawBitmapString( "(f)ullframe/window", 30, data->windowHeight - 20);
    ofDrawBitmapString( "(a)udio/soundtrack", 30, data->windowHeight - 40); ofDrawBitmapString(":", 180, data->windowHeight - 40);
    if (data->bAudioOn) ofDrawBitmapString("ON", 220, data->windowHeight - 40); else ofDrawBitmapString("OFF", 220, data->windowHeight - 40);
    ofDrawBitmapString( "(s)how data", 30, data->windowHeight - 60);
    
    ofDrawBitmapString( "sounds playing: " + ofToString((float)data->soundsPlaying, 0), 30, data->windowHeight - 100);
    ofDrawBitmapString( "videos playing: " + ofToString((float)data->videosPlaying, 0), 30, data->windowHeight - 120);
    
    ofDrawBitmapString( "fps: " + ofToString(ofGetFrameRate(), 0), data->windowWidth - 100, 30);
    ofDrawBitmapString( "frame number: " + ofToString(data->frameNumber, 0), data->windowWidth - 300, 30);
    ofDrawBitmapString( "seconds: " + ofToString((float)data->frameNumber/FRAME_RATE, 2), data->windowWidth - 450, 30);
    

    ofPopMatrix();
    
    ofSetColor(255);
    
    glPopMatrix();
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
        if ((*it)->sound.getIsPlaying()) data->soundsPlaying++;
    }
    
}


void autoPlayerApp::loadConfig(){
    
    
    data->message = "loading config.xml";
    if( data->config.loadFile(ofFilePath::getAbsolutePath(DEFAULT_CONFIG, TRUE).c_str())){data->message = ofToString(DEFAULT_CONFIG) + " loaded!";}
    else{data->message = "unable to load " + ofToString(DEFAULT_CONFIG) + ", check data/ folder";}
    
    
    /*
    message = "loading mySettings.xml";
    
    //we load our settings file
    //if it doesn't exist we can still make one
    //by hitting the 's' key
    if( XML.loadFile("mySettings.xml") ){
        message = "mySettings.xml loaded!";
    }else{
        message = "unable to load mySettings.xml check data/ folder";
    }
    */
    
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
            
            if (node->ValueStr()=="Global") {
                
                if (element->ValueStr()=="Title") {element->QueryStringAttribute("title", &data->title);}
                
                if (element->ValueStr()=="Window") {
                    element->QueryIntAttribute("width", &data->windowWidth); data->origWindowWidth = data->windowWidth;
                    element->QueryIntAttribute("height", &data->windowHeight); data->origWindowHeight = data->windowHeight;
                    string tempString; element->QueryStringAttribute("fullscreen", &tempString); tempString = ofToLower(tempString);
                    if (tempString == "true") data->bFullScreen = true; else data->bFullScreen = false;
                }
                
                if (element->ValueStr()=="Tiles") {
                    element->QueryIntAttribute("horizontal",  &data->tiles_h);
                    element->QueryIntAttribute("vertical",  &data->tiles_v);
                }
                
                if (element->ValueStr()=="Background") {
                    element->QueryFloatAttribute("red", &data->backgroundColor.r);
                    element->QueryFloatAttribute("green", &data->backgroundColor.g);
                    element->QueryFloatAttribute("blue", &data->backgroundColor.b);
                }
                
                if (element->ValueStr()=="Playtype") {
                    string tempString; element->QueryStringAttribute("type", &tempString); tempString = ofToLower(tempString);
                    if (tempString == "once") data->playType = ONCE; if (tempString == "loop") data->playType = LOOP; if (tempString == "endless") data->playType = ENDLESS;
                    element->QueryIntAttribute("duration", &data->playDuration); data->playDuration = data->playDuration * FRAME_RATE;
                    element->QueryStringAttribute("playtransition", &tempString); tempString = ofToLower(tempString);
                    if (tempString == "true") {data->bPlayTransition = true;} else {data->bPlayTransition = false; data->globalAlpha = 1;}
                }
                
                if (element->ValueStr()=="Videos") { element->QueryIntAttribute("max", &data->videosPlayingMax);}
                if (element->ValueStr()=="Sounds") { element->QueryIntAttribute("max", &data->soundsPlayingMax);}
                
                if (element->ValueStr()=="ShowData") {
                    string tempString; element->QueryStringAttribute("showdata", &tempString); tempString = ofToLower(tempString);
                    if (tempString == "true") data->bShowData = true; else data->bShowData = false;
                }
                
                
                if (element->ValueStr()=="InputType") {
                    string tempString; element->QueryStringAttribute("input", &tempString); tempString = ofToLower(tempString);
                    if (tempString == "mouse") data->inputType = MOUSE; else if (tempString == "cameras")  data->inputType = CAMERAS; else if (tempString == "off")  data->inputType = OFF;
                }
            }
            
            
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
}
