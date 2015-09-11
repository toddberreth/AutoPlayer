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

#include "ofMain.h"
#include "defines.h"
#include "autoPlayerData.h"
#include "autoPlayerMedia.h"

class autoPlayerApp : public ofBaseApp{

	public:
    
		void setup();
		void update();
		void draw();
    
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    private:
    
        void loadConfig();
        void loadEvent(autoPlayerMedia *event, int i);
        void countPlayingMedia();
    
        autoPlayerData				*data;
    
        vector<autoPlayerMedia*>    events;
    
        bool                        bMouseMoved;
        int                         mouseTimer;
};
