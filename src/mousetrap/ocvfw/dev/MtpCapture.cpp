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

#include <iostream>
#include <map>
#include <stdio.h>
#include <glibmm.h>

#include "cv.h"
#include "highgui.h"
#include "MtpCamera.h"
#include "MtpCapture.h"

using namespace std;

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
	this->img = this->webcam.queryFrame();

	if (!this->img)
		printf("Not Image loaded\n");
		return true;

	return true;
}

IplImage *MtpCapture::image() {
	return this->img;
}

IplImage *MtpCapture::rect(CvRect rect) {
	IplImage *p_rect;
	CvMat p_mat;


	p_rect = (IplImage *) cvAlloc(sizeof(IplImage));

	cvGetSubRect(this->img, &p_mat, rect);

	p_rect = cvGetImage(&p_mat, p_rect);

	return p_rect;
}

IplImage *MtpCapture::color(int channel, int new_color, bool copy) {
	IplImage *tmp;

	tmp = cvCreateImage( cvGetSize(this->img), 8,  channel);

	cvCvtColor(this->img, tmp, new_color);

	if (!copy)
		this->img = tmp;

	return tmp;
}

void MtpCapture::flip(int flip) {
	if (flip <= 1)
		cvFlip(this->img, this->img, 1);

	if (flip >= 1)
		cvFlip(this->img, this->img, 0);
}

IplImage *MtpCapture::resize(int width, int height, bool copy) {
	IplImage *tmp;

	tmp = cvCreateImage(cvSize(width, height), 8, this->img->nChannels);
	cvResize(this->img, tmp, CV_INTER_AREA);

	if (!copy)
		this->img = tmp;

	return tmp;
}
