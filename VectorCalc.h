#pragma once
#ifndef _NSTD_VECTOR_CALCULUS_
#define _NSTD_VECTOR_CALCULUS_

#include "Defines.h"

#ifdef _GEOMETRY_
#pragma message("_NSTD_VECTOR_CALCULUS_ does not function with _GEOMETRY_, please make sure not to include _GEOMETRY_ when working with this file.")
#else // ^^^^ _GEOMETRY_ / !_GEOMETRY_ vvvv

_NSTD_BEGIN
#define _NUM_DIMENSIONS_ 3

struct Point {
	Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

	void dot_prod(Point p1, Point p2) {
		_NSTD_FOR(_NUM_DIMENSIONS_)
			* mem_arr[i] = (p1.*mem_arr)[i];
	}

	void cross_prod(Point p1, Point p2) {

	}

	double (Point::* mem_arr)[] = {&Point::x, &Point::y, &Point::z};
	double x, y, z;
};

struct Vector {
	Vector(Point origin = Point, double i = 0, double j = 0, double k = 0) : origin_(origin), i(i), j(j), k(k) {}


	Point origin_;
	double i, j, k; // i hat, j hat, k hat
};






















_NSTD_END

#endif // _GEOMETRY_
#endif // _NSTD_VECTOR_CALCULUS_