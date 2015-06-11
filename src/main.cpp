#include "ofMain.h"
#include "autoPlayerApp.h"


//========================================================================
int main( ){
	
    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    ofSetupOpenGL(&window, WINDOW_WIDTH_DEFAULT,WINDOW_HEIGHT_DEFAULT,OF_WINDOW);
	ofRunApp(new autoPlayerApp());
}
