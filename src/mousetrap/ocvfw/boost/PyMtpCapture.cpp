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
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

using namespace std;
using namespace cv;
using namespace Mtp;
using namespace boost::python;

// Defining memebers overloads
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(color_overloads, color, 2, 3)

BOOST_PYTHON_MODULE(camera)
{
	class_<Mat>("Mat", init<>())
		.def_readwrite("cols", &Mat::cols )
		.def_readwrite("rows", &Mat::rows )
		.def_readwrite("step", &Mat::step )
		//.def_readwrite( "data", &Mat::data )
		.def_readwrite( "flags", &Mat::flags )
		.def_readwrite( "dataend", &Mat::dataend )
		.def_readwrite( "refcount", &Mat::refcount )
		.def_readwrite( "datastart", &Mat::datastart )

//		.staticmethod( "diag" )
//		.staticmethod("from_ndarray")

		.def( init< Size, int >() )
		.def( init< Mat const & >() )
		.def( init< int, int, int >() )
		.def( init< Size, int, Scalar const & >() )
		.def( init< Mat const &, Rect const & >() )
		.def( init< int, int, int, Scalar const & >() )
		.def( init< Size, int, void *, optional< size_t > >() )
		.def( init< int, int, int, void *, optional< size_t > >() )
		.def( init< Mat const &, Range const &, Range const & >() )

		.def("col", &Mat::col)
		.def("row", &Mat::row)
		.def("size", &Mat::size)
		.def("type", &Mat::type)
		.def("empty", &Mat::empty)
		.def("clone", &Mat::clone)
		.def("depth", &Mat::depth)
		//.def("colRange", &Mat::colRange)
		.def("assignTo", &Mat::assignTo)
		.def("channels", &Mat::channels)
		//.def("from_ndarray", &as_Mat);
		//.def("adjustROI", &Mat::adjustROI)
	;

	class_<Capture>("Capture", init<>())
			.def("color", &Capture::color, color_overloads())
			.def("flip", &Capture::flip)
			//.def("image", &Capture::image)
			.def("init", &Capture::init)
			.def("rect", &Capture::rect)
			.def("resize", &Capture::resize)
			.def("set_async", &Capture::set_async)
			.def("sync", &Capture::sync)
			.def("get_haar_points", &Capture::getHaarPointsPython)
	;
}
