import os
from distutils.core import setup                                                 
from distutils.core import Extension

os.environ['CC'] = 'g++ -shared -fpic' 
os.environ['CXX'] = 'g++ -shared -fpic'
os.environ['CPP'] = 'g++ -shared -fpic'
os.environ['LDSHARED'] = 'g++ -shared -fpic'

extra_options = {'build': ['-shared', '-fpic']}

module = Extension('capture',
                    libraries = ['cv', 'highgui', 'cxcore', 'cvaux', 'ml', 'gobject-2.0', 'gmodule-2.0', 'gthread-2.0', 'rt', 'xml2', 'glib-2.0', 'glibmm-2.4', 'sigc-2.0'],
                    include_dirs=['/usr/include/opencv', 
                                  '/usr/include/glib-2.0/', 
                                  '/usr/lib/glib-2.0/include/',
                                  '/usr/include/glibmm-2.4/', 
                                  '/usr/lib/glibmm-2.4/include/', 
                                  '/usr/include/libxml2/',
                                  '/usr/include/sigc++-2.0/',
                                  '/usr/include/sigc++-2.0/sigc++/',
                                  '/usr/lib/sigc++-2.0/include',
                                  './include/'],
                    sources = ['backends/OcvfwBase.cpp','dev/MtpCamera.cpp', 'dev/MtpCapture.cpp', 'dev/PyMtpCapture.cpp'],)

setup(name = 'capture',
      version = '0.1',
      ext_modules = [module])