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
#include "ofxOsc.h"
#include "defines.h"


using namespace std;

class CamBlob
{
public:

	CamBlob(){};
	~CamBlob();

    unsigned int            id;
    float                   centroidX;          // centroid - X axis 0.0->1.0 within field of view
    float                   centroidY;          // centroid - Y axis 0.0->1.0 within field of view

    float                   targetCentroidX; 	// target centroid - X
    float                   targetCentroidY;	// target centroid - Y

	bool                    blobActive;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int            remove_counter;		// used by master node - tally the program cycles where blob is inactive
                                                //deleted after x cycles.
};


struct ControlStruct
{
	bool                    occupied;
	std::vector<CamBlob*>	camBlobs;
	float                   blobs[8][2];
};


class CamData{

private:

public:

	std::vector<CamBlob*>   myBlobs;
	bool                    occupied;

	void setup();
	void update();
    
    bool isOccupied(){return sharedControl->occupied;}

    ControlStruct           *sharedControl;

    ofxOscReceiver          receiver;
    
};

