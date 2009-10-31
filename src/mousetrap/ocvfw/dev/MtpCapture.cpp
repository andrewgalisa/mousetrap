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

#include "pthread.h"

#include "MtpCapture.h"

bool async = false;
pthread_t thread;

MtpCapture::MtpCapture() {
	webcam.startCamera();
}

void MtpCapture::init() {
   if(capture!=0)
   cvReleaseCapture( &capture );
}

int MtpCapture::set_async(bool val) {
	int rc;

	if ( val == true) {
		rc = pthread_create(&thread, NULL, sync());

		if (rc){
		 printf("ERROR; return code from pthread_create() is %d\n", rc);
		 return 1;
		}
	} else {
		pthread_exit(0);
	}

	return 0;
}

int MtpCapture::sync() {
	image = webcam.queryFrame();

	if (!image)
		return 1;

	return 0;
}
