#pragma once
#ifndef _NSTD_VECTOR_CALCULUS_
#define _NSTD_VECTOR_CALCULUS_

#include "Defines.h"
#include "ArrayManager.h"

#ifdef _GEOMETRY_
#pragma message("_NSTD_VECTOR_CALCULUS_ does not function with _GEOMETRY_, please make sure not to include _GEOMETRY_ when working with this file.")
#else // ^^^^ _GEOMETRY_ / !_GEOMETRY_ vvvv

_NSTD_BEGIN
#define _NSTD_VC_ND 3


struct Point {
	Point(double* arr = NULL /* Assumed size of _NSTD_VC_ND */) {
		arr_ = new double[_NSTD_VC_ND];
		if (arr) {
			_NSTD_FOR(_NSTD_VC_ND)
				arr_[i] = arr[i];
		} else {
			_NSTD_FOR(_NSTD_VC_ND)
				arr_[i] = 0;
		}
	}
	~Point() {
		delete[] arr_;
	}

	Point& dot_prod(Point p1, Point p2) {
		_NSTD_FOR(_NSTD_VC_ND)
			arr_[i] = p1.arr_[i] * p2.arr_[i];
		return *this;
	}

	Point& cross_prod(Point p1, Point p2) {

		return *this;
	}

	
	double* arr_; // Size _NSTD_VC_ND
};

struct Vector {
	Vector(Point origin = Point(), Point vector = Point()) : origin_(origin), vect_(vector) {}


	Point origin_, vect_;
};






















_NSTD_END

#endif // _GEOMETRY_
#endif // _NSTD_VECTOR_CALCULUS_