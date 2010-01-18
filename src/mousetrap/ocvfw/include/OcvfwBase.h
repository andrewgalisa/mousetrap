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

#ifndef OCVFWBASE_H_
#define OCVFWBASE_H_

/**
 * Image Width of Webcam
 */
#define IMAGE_WIDTH 320

/*
 * Image Height of Webcam
 */
#define IMAGE_HEIGHT 240

/*
 * Image Height of Webcam
 */
#define SMALL_FRAME_SCALE 1.5

/**
 * Base Framework class.
 */
class OcvfwBase
{
public:

    /**
     * The Constructor
     * Currently does nothing
     */
    OcvfwBase(void);

    /**
     * OpenCV WaitKey call
     */
    int waitKey(int num);

    /**
     * Creates a new image. Currently just calls OpenCv CreateImage
     *
     * @param size The image size
     * @param depth The image depth
     * @param channels The number of channels.
     *
     * @return A pointer to the new image.
     */
    IplImage *newImage(CvSize size, int depth, int channels);


    /**
     *Query Image From Webcam
     *@result Image From the Webcam
     */
    IplImage *queryFrame();

    /**
     *Initialize Webcam for Querying Image
     *@result returns 1 for Sucess and 0 for Failure
     */
    int startCamera(int idx);

    /**
     *Deinitialize Webcam Structures
     */
    void stopCamera();


private:
    /**
    * Opencv Capture Structure
    */
    CvCapture* capture;

    /**
    * Opencv Mem Storage
    */
    CvMemStorage* storage;

    /**
     * The camera index.
     */
    int idx;
};

#endif /* OCVFWBASE_H_ */
