/*
 * AutoPlayer
 * https://github.com/toddberreth/AutoPlayer
 *
 * Copyright 2015, Todd Berreth
 *
 * Licensed under the MIT license:
 * http://www.opensource.org/licenses/MIT
 */

#include "autoPlayerCams.h"

ControlStruct myControlStruct;


//--------------------------------------------------------------
void CamData::setup(){

    receiver.setup(CAMERA_OSC_RECEIVE_PORT);
    sharedControl = &myControlStruct;
}

//--------------------------------------------------------------
void CamData::update(){

    while(receiver.hasWaitingMessages()){
        // get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

        if(m.getAddress() == "/blob"){

            float id                = m.getArgAsFloat(0);
            float centroidX         = m.getArgAsFloat(1);
            float centroidY         = m.getArgAsFloat(2);
            float centroidXDiff     = m.getArgAsFloat(3);
            float centroidYDiff     = m.getArgAsFloat(4);
            float centroidDiffDist  = m.getArgAsFloat(5);
            float aspectRatio       = m.getArgAsFloat(6);
            float areaROI           = m.getArgAsFloat(7);
            float areaDiffROI       = m.getArgAsFloat(8);
            float angle             = m.getArgAsFloat(9);
            float angleDiff         = m.getArgAsFloat(10);
            float lifetime          = m.getArgAsFloat(11);
            float blobActive        = m.getArgAsFloat(12);

            //cout << id << "  "  << centroidX << "  " << centroidY << endl;

            bool foundBlob = false;
            for (int x = 0; x < myControlStruct.camBlobs.size(); x++){
                if (myControlStruct.camBlobs[x]->id == id){
                    myControlStruct.camBlobs[x]->centroidX 	= centroidX;
                    myControlStruct.camBlobs[x]->centroidY 	= 1-centroidY;
                    if (blobActive == 1) {myControlStruct.camBlobs[x]->blobActive = true;}
                    else {myControlStruct.camBlobs[x]->blobActive = false;}
                    myControlStruct.camBlobs[x]->remove_counter 	= 0;
                    foundBlob = true;
                    break;
                }
            }
            if (!foundBlob){
                CamBlob* newCamBlob = new CamBlob();
                newCamBlob->id 			= id;
                newCamBlob->centroidX 	= (float)centroidX;
                newCamBlob->centroidY 	= 1-(float)centroidY;
                if (blobActive == 1) {newCamBlob->blobActive = true;}
                else {newCamBlob->blobActive = false;}
                newCamBlob->remove_counter 	= 0;
                myControlStruct.camBlobs.push_back(newCamBlob);
            }
        }
    }

    if (myControlStruct.camBlobs.size() > 0){
        for(std::vector<CamBlob*>::iterator it=myControlStruct.camBlobs.begin(); it != myControlStruct.camBlobs.end(); ){
            if ((*it)->blobActive == false) myControlStruct.camBlobs.erase(it);else {it++;}
        }
    }

    myControlStruct.occupied = false;

    for (int x = 0; x < 8; x++){
        bool match = false;
        for (int y = 0; y < myControlStruct.camBlobs.size(); y++){
            if (myControlStruct.camBlobs[y]->id == x){
                myControlStruct.blobs[x][0] = myControlStruct.camBlobs[y]->centroidX;
                myControlStruct.blobs[x][1] = myControlStruct.camBlobs[y]->centroidY;
                myControlStruct.occupied = true;
                match = true;
                break;
            }
        }
        if (!match) {
            myControlStruct.blobs[x][0] = -1;
            myControlStruct.blobs[x][1] = -1;
        }
    }

    for (int x = 0; x < 8; x++){

        bool foundBlob = false;
        std::vector<CamBlob*>::iterator it;
        for(it = myBlobs.begin(); it != myBlobs.end(); it++){
            if ((*it)->id == x){foundBlob = true;break;}
        }

        if (foundBlob){
            if (myControlStruct.blobs[x][0] >= 0){
                (*it)->targetCentroidX = myControlStruct.blobs[x][0];
                (*it)->targetCentroidY = myControlStruct.blobs[x][1];
                (*it)->centroidX = CAMERA_BLOB_SMOOTH_COEF*(*it)->targetCentroidX + (1-CAMERA_BLOB_SMOOTH_COEF)*(*it)->centroidX;
                (*it)->centroidY = CAMERA_BLOB_SMOOTH_COEF*(*it)->targetCentroidY + (1-CAMERA_BLOB_SMOOTH_COEF)*(*it)->centroidY;
            }
            else {
                myBlobs.erase(it);
            }
        }
        else{
            if (myControlStruct.blobs[x][0] >= 0){
                CamBlob *newCamBlob = new CamBlob();
                newCamBlob->id                  = x;
                newCamBlob->centroidX           = myControlStruct.blobs[x][0];
                newCamBlob->centroidY           = myControlStruct.blobs[x][1];
                newCamBlob->targetCentroidX 	= newCamBlob->centroidX;
                newCamBlob->targetCentroidY 	= newCamBlob->centroidY;
                myBlobs.push_back(newCamBlob);
            }
        }
    }

    if (myBlobs.size() > 0 ) {myControlStruct.occupied = true; occupied = true;}
    else {myControlStruct.occupied = false; occupied = false;}
}
