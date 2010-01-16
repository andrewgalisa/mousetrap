/**
 * Ocvfw
 *
 * Copyright 2009 Flavio Percoco Premoli
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

#include "Capture.h"

#include "Python.h"
#include "structmember.h"

static Capture cap;

typedef struct {
	PyObject_HEAD
} Capture;

static void Capture_dealloc(Capture* self) {
	self->ob_type->tp_free((PyObject*) self);
}

static PyObject *
Ipl2PyDict(IplImage *frame) {
	PyObject *tmp;

    if (!frame)
        return Py_None;

	tmp = PyDict_New();
	PyDict_SetItemString( tmp, "width",     PyInt_FromLong( frame->width     ) );
	PyDict_SetItemString( tmp, "height",    PyInt_FromLong( frame->height    ) );
	PyDict_SetItemString( tmp, "widthStep", PyInt_FromLong( frame->widthStep ) );
	PyDict_SetItemString( tmp, "depth",     PyInt_FromLong( frame->depth    ) );
	PyDict_SetItemString( tmp, "imageData", PyString_FromStringAndSize(frame->imageData, frame->imageSize) );

	return tmp;
}

static PyObject *
Capture_image(Capture *self) {
	//return Ipl2PyDict(cap.image());
	return Ipl2PyDict(cap.image());
}

static PyObject *
Capture_rect(Capture *self, PyObject *args, PyObject *keywds) {
	int x, y, width, height;
	PyObject *tmp;

	static char *kwlist[] = { "x", "y", "width", "height", NULL };

	PyArg_ParseTupleAndKeywords(args, keywds, "iiii", kwlist, &x, &y, &width, &height);

	tmp = Ipl2PyDict(cap.rect(cvRect(x, y, width, height)));

	return tmp;

}

static PyObject *
Capture_color(Capture *self, PyObject *args, PyObject *keywds) {
	int new_color, channels, copy;

	static char *kwlist[] = { "new_color", "channels", "copy", NULL };

	PyArg_ParseTupleAndKeywords(args, keywds, "ii|i", kwlist, &channels, &new_color, &copy);

	return Ipl2PyDict(cap.color(channels, new_color, (copy == 0) ? false : true));

}

static PyObject *
Capture_resize(Capture *self, PyObject *args, PyObject *keywds) {
	int width, height, copy = 0;

	static char *kwlist[] = { "width", "height", "copy", NULL };

	PyArg_ParseTupleAndKeywords(args, keywds, "ii|i", kwlist, &width, &height, &copy);

	return Ipl2PyDict(cap.resize(width, height, (copy == 0) ? false : true));

}

static PyObject *
Capture_sync(Capture *self) {
	cap.sync();

	return Py_None;
}

static PyObject *
Capture_set_async(Capture *self, PyObject *args, PyObject *keywds) {
	int fps = 100, async=0;

	static char *kwlist[] = { "fps", "async", NULL };

	PyArg_ParseTupleAndKeywords(args, keywds, "|ii", kwlist, &fps, &async);

	cap.set_async(fps, (async == 0) ? false : true);
	return Py_None;
}

static PyObject *
Capture_flip(Capture *self, PyObject *args, PyObject *keywds) {
	int flip;

	PyArg_ParseTuple(args, "i", &flip);

	cap.flip(flip);
	return Py_None;
}


static PyObject *
Capture_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	Capture *self;
	self = (Capture *) type->tp_alloc(type, 0);

	return (PyObject *) self;
}

static int Capture_init(Capture *self, PyObject *args, PyObject *keywds) {
	int fps = 100, idx = 0, async = 0;

	static char *kwlist[] = { "fps", "async", "idx", NULL };

	PyArg_ParseTupleAndKeywords(args, keywds, "|iii", kwlist, &fps, &async, &idx);

	cap.init(fps, (async == 0) ? false : true, idx);

	return 0;
}

static PyMemberDef Capture_members[] = {
		{ NULL } /* Sentinel */
};

static PyMethodDef Capture_methods[] = {
		{ "set_async", (PyCFunction) Capture_set_async, METH_KEYWORDS, PyDoc_STR("Enables disables asynchronous calls to sync method.") },
		{ "sync", (PyCFunction) Capture_sync, METH_NOARGS, PyDoc_STR("The Sync method that queries new frames.") },
		{ "image", (PyCFunction) Capture_image, METH_NOARGS, PyDoc_STR("Returns the current IplDict object.") },
		{ "resize", (PyCFunction) Capture_resize, METH_KEYWORDS, PyDoc_STR("Resizes the current image and returns it.") },
		{ "rect", (PyCFunction) Capture_rect, METH_KEYWORDS, PyDoc_STR("Returns the required rectangle of the image.") },
		{ "flip", (PyCFunction) Capture_flip, METH_VARARGS, PyDoc_STR("Flips the image") },
		{ "color", (PyCFunction) Capture_color, METH_KEYWORDS, PyDoc_STR("Returns the pointer to the converted image.") },
		{NULL } /* Sentinel */
};

static PyTypeObject CaptureType = { PyObject_HEAD_INIT(NULL)0, /*ob_size*/
		"Capture", /*tp_name*/
		sizeof(Capture), /*tp_basicsize*/
		0, /*tp_itemsize*/
		(destructor)Capture_dealloc, /*tp_dealloc*/
		0, /*tp_print*/
		0, /*tp_getattr*/
		0, /*tp_setattr*/
		0, /*tp_compare*/
		0, /*tp_repr*/
		0, /*tp_as_number*/
		0, /*tp_as_sequence*/
		0, /*tp_as_mapping*/
		0, /*tp_hash */
		0, /*tp_call*/
		0, /*tp_str*/
		0, /*tp_getattro*/
		0, /*tp_setattro*/
		0, /*tp_as_buffer*/
		Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
		"Capture objects", /* tp_doc */
		0, /* tp_traverse */
		0, /* tp_clear */
		0, /* tp_richcompare */
		0, /* tp_weaklistoffset */
		0, /* tp_iter */
		0, /* tp_iternext */
		Capture_methods, /* tp_methods */
		Capture_members, /* tp_members */
		0, /* tp_getset */
		0, /* tp_base */
		0, /* tp_dict */
		0, /* tp_descr_get */
		0, /* tp_descr_set */
		0, /* tp_dictoffset */
		(initproc)Capture_init, /* tp_init */
		0, /* tp_alloc */
		Capture_new, /* tp_new */
};

static PyMethodDef module_methods[] = { { NULL } /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC initcapture(void) {
	PyObject* m;

	if (PyType_Ready(&CaptureType) < 0)
		return;

	m = Py_InitModule3("capture", module_methods,
			"Example module that creates an extension type.");

	if (m == NULL)
		return;

	Py_INCREF(&CaptureType);
	PyModule_AddObject(m, "Capture", (PyObject *) &CaptureType);
	//Capture_error = PyErr_NewException("Capture.error", NULL, NULL);
}
