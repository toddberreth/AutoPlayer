#include "ofMain.h"
#include "autoPlayerApp.h"


//========================================================================
int main( ){
	
    ofSetupOpenGL(WINDOW_WIDTH_DEFAULT,WINDOW_HEIGHT_DEFAULT,OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new autoPlayerApp());
}
