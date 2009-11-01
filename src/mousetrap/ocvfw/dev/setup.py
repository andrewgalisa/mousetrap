from distutils.core import setup                                                 
from distutils.core import Extension

module2 = Extension('capture',
                    libraries = ['cv', 'highgui', 'cxcore', 'cvaux', 'ml', 'gstreamer-0.10', 'gobject-2.0', 'gmodule-2.0', 'gthread-2.0', 'rt', 'xml2', 'glib-2.0'],
                    include_dirs=['/usr/include/opencv', '/usr/include/gstreamer-0.10/', '/usr/include/glib-2.0/', '/usr/lib/glib-2.0/include/', '/usr/include/libxml2/'],
                    sources = ['MtpCamera.cpp', 'MtpCapture.cpp', 'PyMtpCapture.cpp'])

setup(name = 'capture',
      version = '0.1',
      ext_modules = [module2])