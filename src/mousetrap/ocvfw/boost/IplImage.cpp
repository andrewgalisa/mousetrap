#define PY_ARRAY_UNIQUE_SYMBOL PyArrayXXX

#include <stdio.h>
#include <iostream>
#include <string>

#include <boost/python.hpp>
#include <boost/python/numeric.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "numpy_boost.hpp"
// #include "pymultiarray.hpp"

using namespace std;
using namespace boost::python;


/****************************************************************************
 * Utility functions to convert between NumPy and OpenCV data-structures
 */

/// Converts an IplImage into a python array with a single assginment
PyObject* cv2py(IplImage* img)
{
    npy_intp* shape = new npy_intp[3];
    shape[0] = img->height;
    shape[1] = img->width;
    shape[2] = img->nChannels;

    char type;
    switch(img->depth)
    {
        case IPL_DEPTH_8U : type = NPY_UBYTE ; break;
        case IPL_DEPTH_8S : type = NPY_BYTE  ; break;
        case IPL_DEPTH_16U: type = NPY_USHORT; break;
        case IPL_DEPTH_16S: type = NPY_SHORT ; break;
        case IPL_DEPTH_32S: type = NPY_INT   ; break;
        case IPL_DEPTH_32F: type = NPY_FLOAT ; break;
        default           : type = NPY_DOUBLE; break;
    }

    PyObject* obj = PyArray_SimpleNewFromData(3, shape, type, img->imageData);

    PyArrayObject* arr = (PyArrayObject*)obj;

    int typesize = sizeof(img->imageData[0]);
    arr->strides[0] = img->widthStep;
    arr->strides[1] = img->nChannels*typesize;
    arr->strides[2] = typesize;

    return (PyObject*)arr;
}

/// Converts a python array into an IplImage with a single assginment
IplImage* py2cv(PyObject* obj)
{
    PyArrayObject* arr = (PyArrayObject*)obj;

    int depth;
    switch(arr->descr->type_num)
    {
        case NPY_UBYTE : depth = IPL_DEPTH_8U ; break;
        case NPY_BYTE  : depth = IPL_DEPTH_8S ; break;
        case NPY_USHORT: depth = IPL_DEPTH_16U; break;
        case NPY_SHORT : depth = IPL_DEPTH_16S; break;
        case NPY_INT   : depth = IPL_DEPTH_32S; break;
        case NPY_FLOAT : depth = IPL_DEPTH_32F; break;
        default        : depth = IPL_DEPTH_64F; break;
    }

    IplImage* img = cvCreateImage(cvSize(arr->dimensions[1], arr->dimensions[0]),
                                         depth, arr->dimensions[2]);

    img->imageData = arr->data;

    return img;
}

/*****************************************************************************
* FireWire Camera Class
*/
class CvFWCamera
{
    private:
        // OpenCV capture structure
        CvCapture *_capture;
        int _width, _hight;

    public:
        /**
        * Constructor
        */
        CvFWCamera(int w, int h)
        {
            _width = w;
            _hight = h;
            _capture = cvCreateCameraCapture(0);

            if (!_capture) {
                throw std::runtime_error("Could not create camera capture");
            }

            cvSetCaptureProperty(_capture,CV_CAP_PROP_FPS, 7.5);
        }

        /**
        * Capture next frame and return a tuple containing two NumPy arrays.
        * One beeing the gray image, the other one beeing the RGB image.
        */
        tuple next_frame()
        {
            IplImage *frame;
            IplImage *image;

            // If out capture source really open?
            if (!_capture) {
                throw std::runtime_error("File not open");
            }

            // Get next frame
            frame = cvQueryFrame(_capture);

            if (!frame) {
                throw std::runtime_error("Could not capture frame from Camera");
            }

            image = cvCreateImage(cvSize(_width, _hight),
                                           frame->depth, frame->nChannels);

            cvResize(frame, image);

            // Convert to gray image
            IplImage* gray_image = cvCreateImage(cvSize(image->width,image->height),
                                                 IPL_DEPTH_8U, 1);

                                                 cvCvtColor(image, gray_image, CV_BGR2GRAY);

                                                 // Convert to NumPy arrays
                                                 boost::python::handle<> gray_arr( cv2py(gray_image) );
                                                 boost::python::handle<> rgb_arr( cv2py(image) );

                                                 // Make and return a tuple
                                                 return make_tuple(gray_arr, rgb_arr);
        }
};

/*****************************************************************************
* Use boost-python to actually export classes and functions...
*/
BOOST_PYTHON_MODULE(fwcamera_ext) {
    import_array();

    class_<CvFWCamera>("CvFWCamera", init<int, int>())
    .def("next_frame", &CvFWCamera::next_frame)
    ;
}
