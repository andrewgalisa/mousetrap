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

#include "Python.h"
#include "cv.h"
#include "highgui.h"

#include "structmember.h"

#include "MtpCapture.h"

static MtpCapture cap;

typedef struct {
	PyObject_HEAD
} Capture;

static void Capture_dealloc(Capture* self) {
	self->ob_type->tp_free((PyObject*) self);
}

static PyObject *
Capture_set_async(Capture *self, PyObject *args, PyObject *keywds) {
	int fps = 100, async=1;

	static char *kwlist[] = { "fps", "async", NULL };

	PyArg_ParseTupleAndKeywords(args, keywds, "ii", kwlist, &fps, &async);

	cap.set_async(fps, (async == 0) ? true : false);
	return Py_None;
}

static PyObject *
Capture_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	Capture *self;
	self = (Capture *) type->tp_alloc(type, 0);

	return (PyObject *) self;
}

static int Capture_init(Capture *self, PyObject *args, PyObject *keywds) {
	int fps = 100, idx = 0, async = 1;

	static char *kwlist[] = { "fps", "async", "idx", NULL };

	PyArg_ParseTupleAndKeywords(args, keywds, "|iii", kwlist, &fps, &async, &idx);

	cap.init(fps, (async == 0) ? true : false, idx);

	return 0;
}

static PyMemberDef Capture_members[] = { { NULL } /* Sentinel */
};

static PyMethodDef Capture_methods[] = { { "set_async",
		(PyCFunction) Capture_set_async, METH_VARARGS|METH_KEYWORDS,
		PyDoc_STR("Enables disables asynchronous calls to sync method.") }, {
		NULL } /* Sentinel */
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
