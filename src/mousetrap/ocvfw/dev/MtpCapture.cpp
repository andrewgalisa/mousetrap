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

#include <stdio.h>
#include <glibmm.h>

#include "MtpCapture.h"

MtpCapture::MtpCapture() {

}

void MtpCapture::init(int set_fps, bool set_async, int idx) {
	fps = set_fps;
	async = set_async;

	this->webcam.startCamera(idx);
	this->syncSlot = sigc::mem_fun(*this, &MtpCapture::sync);

}

void MtpCapture::set_async(int set_fps, bool set_async) {
	fps = set_fps;
	async = set_async;

	if ( set_async == true) {
		this->timer = Glib::signal_timeout().connect(this->syncSlot, fps);
	} else {
		this->timer.disconnect();
	}
}

bool MtpCapture::sync() {
	this->image = this->webcam.queryFrame();

	if (!this->image)
		return true;

	printf( "%d", this->image->height);

	return true;
}

IplImage *MtpCapture::resize(int width, int height, bool copy) {
	IplImage *tmp;

	tmp = cvCreateImage(cvSize(width, height), 8, this->image->nChannels);
	cvResize(this->image, tmp, CV_INTER_AREA);

	if (!copy)
		this->image = tmp;

	return tmp;
}
