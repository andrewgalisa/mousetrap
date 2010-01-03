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



#include <stdio.h>
#include <glibmm.h>


#include "cv.h"
#include "highgui.h"
#include "MtpCamera.h"

/**
 * OpenCV Webcam Class. This class wraps over the opencv Image Query Functions.
 */
class MtpCapture
{
public:

	/**
	 * FPS
	 */
	int fps;

	/**
	 * Whether it has to query images asynchronously
	 */
	bool async;

	/**
	 * The Constructor
	 * Currently does nothing
	 */
	MtpCapture(void);

	/**
	 * Used to init the camera and capture vars
	 *
	 * @param set_fps The how frequently should be called the sync method.
	 * @param set_async Whether to set or not the asynchronous calls.
	 * @param idx The Camera Index.
	 */
	void init(int set_fps=100, bool set_async=false, int idx=0);

	/**
	 * Resizes the image
	 *
	 * @param width The new image width
	 * @param height The new image height
	 * @param copy Whether to create a new image with the new size
	 * 				or replace the current one.
	 * @return the resized image.
	 *
	 */
	IplImage *resize(int width, int height, bool copy=false);

	/**
	 * Returns the image and allows users to change the
	 * current image pointer.
	 */
	IplImage *image();

	/**
	 * Synchronize the capture quering a new frame.
	 */
	bool sync();

	/**
	 * Starts/Stops the asynchronous calls to the sync method.
	 *
	 * @param set_fps The how frequently should be called the sync method.
	 * @param set_async Whether to set or not the asynchronous calls
	 */
	void set_async(int set_fps=100, bool set_async=false);

	/**
	 * Gets and Returns the required rectangle of the image.
	 *
	 * @param rect A CvRect object with the new rectangle params.
	 * @returns  The pointer to the IplImage rectangle.
	 */
	IplImage *rect(CvRect rect);

	/**
	 * Changes the color of the current image frame.
	 *
	 * @param new_color The new color to set. (Use CV globals).
	 * @param copy Whether to copy the image or not.
	 *
	 * @returns The pointer to the converted image.
	 */
	IplImage *color(int channel, int new_color, bool copy=false);

	/**
		 * Flips the image
		 *
		 * @param flip What axis should be flipped.
		 * 0 horizontally
		 * 1 horizontal and vertically
		 * 2 vertically
		 *
		 */
		void flip(int flip);

private:
	/**
	 *Opencv Capture Structure
	 */
	IplImage* img;

	/**
	 * Camera Object
	 */
	MtpCamera webcam;


	/**
	 * Timer Slot
	 */
	sigc::slot<bool> syncSlot;

	/**
	 * The timer
	 */
	sigc::connection timer;

};

