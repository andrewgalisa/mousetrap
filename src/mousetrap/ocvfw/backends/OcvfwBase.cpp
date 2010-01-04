/**
 * Ocvfw
 *
 * Copyright 2009 Flavio Percoco Premoli
 * Copyright (C) 2009 Rohan Anil (rohan.anil@gmail.com)
 *
 * This file is part of Ocvfw.
 *
 * Ocvfw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2 as published
 * by the Free Software Foundation.
 *
 * Ocvfw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ocvfw.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * TODO: set
 * TODO: lk_swap
 * TODO: set_camera_idx
 * TODO: start_camera
 * TODO: query_image
 * TODO: set_lkpoint
 * TODO: clean_lkpoints
 * TODO: show_lkpoints
 * TODO: swap_lkpoints
 */

#include "cv.h"
#include "highgui.h"
#include "OcvfwBase.h"


static IplImage * orginalFrame=0;
IplImage * frame=0;
IplImage * frame_copy=0;

/**
 * Image Width of Webcam
 */
#define IMAGE_WIDTH 320
/*
 * Image Height of Webcam
 */
#define IMAGE_HEIGHT 240



OcvfwBase::OcvfwBase() {
}

void OcvfwBase::stopCamera()
{
   if(capture!=0)
   cvReleaseCapture( &capture );
}

int OcvfwBase::startCamera(int idx)
{
    capture = cvCaptureFromCAM(idx);

    if(capture==0)
    	return 0;
    else
    	return 1;
}

IplImage *OcvfwBase::queryFrame()
{
    orginalFrame = cvQueryFrame( capture );
    if (orginalFrame==NULL)
    	return 0;

    frame = this->newImage( cvSize(IMAGE_WIDTH,IMAGE_HEIGHT), IPL_DEPTH_8U, orginalFrame->nChannels );
    cvResize(orginalFrame,frame, CV_INTER_LINEAR);

    if ( !frame )
        return 0;

    frame_copy = this->newImage( cvSize(frame->width,frame->height), IPL_DEPTH_8U, frame->nChannels );

    if ( frame->origin == IPL_ORIGIN_TL )
        cvCopy( frame, frame_copy, 0 );
    else
        cvFlip( frame, frame_copy, 0 );

    cvReleaseImage(&frame);

    return frame_copy;
}

IplImage *OcvfwBase::newImage(CvSize size, int depth, int channels) {
	return cvCreateImage(size, depth, channels);
}

int OcvfwBase::waitKey(int num) {
	return cvWaitKey(num);
}

int OcvfwBase::getHaarPoints(char* haarclassifier) {
	CvHaarClassifierCascade* cascade;

	cascade = (CvHaarClassifierCascade*)cvLoad(haarclassifier, 0, 0, 0);

	if (cascade) {

	}
}

