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

#include <glib.h>

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
	 */
	void init(int set_fps=100, bool set_async=false, int idx=0);

	/**
	 * Resizes the image
	 */
	IplImage *resize(int width, int height, bool copy=false);

	/**
	 * Synchronize the capture quering a new frame.
	 */
	gboolean sync();

	/**
	 * Starts/Stops the asynchronous calls to the sync method.
	 */
	void set_async(int set_fps=100, bool set_async=false);

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

