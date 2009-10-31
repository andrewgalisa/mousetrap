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

/** @file */

#include "cv.h"
#include "highgui.h"
#include "MtpCamera.h"

//#include "Camera.h"

#ifndef _INCL_GUARD_WEBCAM
#define _INCL_GUARD_WEBCAM

/**
 * OpenCV Webcam Class. This class wraps over the opencv Image Query Functions.
 */
class MtpCapture
{
public:
    /**
     *The Constructor
     *Currently does nothing
     */
	MtpCapture(void);

    /**
     * Synchronize the capture quering a new frame.
     * @result returns 1 for Sucess and 0 for Failure
     */
    int sync();

    /**
     * Starts/Stops the asynchronous calls to the sync method.
     */
    int set_async

private:
    /**
     *Opencv Capture Structure
     */
    IplImage* image;

    /**
     * Camera Object
     */
    MtpCamera webcam;
};
#endif

