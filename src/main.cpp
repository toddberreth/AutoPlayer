/*
 * Insight Engine
 * https://github.com/toddberreth/AutoPlayer
 *
 * Copyright 2015, Todd Berreth
 *
 * Licensed under the MIT license:
 * http://www.opensource.org/licenses/MIT
 */

#include "ofMain.h"
#include "autoPlayerApp.h"

//========================================================================
int main( ){
	
    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    ofSetupOpenGL(&window, WINDOW_WIDTH_DEFAULT,WINDOW_HEIGHT_DEFAULT,OF_WINDOW);
	ofRunApp(new autoPlayerApp());
}
