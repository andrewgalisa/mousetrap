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
#include "MtpCamera.h"

#include "glib.h"

#include "MtpCapture.h"

int fps = 0;
bool async = false;

MtpCapture::MtpCapture() {
	webcam.startCamera();
}

void MtpCapture::init() {
   if(capture!=0)
   cvReleaseCapture( &capture );
}

void MtpCapture::set_async(int set_fps=100, bool set_async=false) {
	fps = set_fps;
	async = set_async;

	if ( set_async == true)
		g_timeout_add(set_fps, sync, NULL);
}

bool MtpCapture::sync() {
	image = webcam.queryFrame();

	if (!image)
		return async;

	return async;
}
