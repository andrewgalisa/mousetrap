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

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "Camera.h"

static IplImage * orginalFrame=0;
IplImage * frame=0;
IplImage * frame_copy=0;

Camera::Camera()
{

}

void Camera::stopCamera()
{
   if(capture!=0)
   cvReleaseCapture( &capture );
}
int Camera::startCamera()
{
    capture =cvCaptureFromCAM(0);
    if(capture==0)
    return 0;
    else
    return 1;
}

IplImage *Camera::queryFrame()
{
    orginalFrame = cvQueryFrame( capture );
    if (orginalFrame==NULL) return 0;
    frame = cvCreateImage( cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U, orginalFrame->nChannels );
    cvResize(orginalFrame,frame, CV_INTER_LINEAR);
    if ( !frame )
        return 0;

    frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
    if ( frame->origin == IPL_ORIGIN_TL )
        cvCopy( frame, frame_copy, 0 );
    else
        cvFlip( frame, frame_copy, 0 );
    cvReleaseImage(&frame);

    return frame_copy;
}
