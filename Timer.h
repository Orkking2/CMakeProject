#pragma once
#ifndef _NSTD_TIMER_
#define _NSTD_TIMER_

#include "Defines.h"
#include <chrono>

_NSTD_BEGIN

class timer {
	_STD chrono::time_point<_STD chrono::steady_clock> start_time_;
public:
	timer() : start_time_(_STD chrono::steady_clock::now()) {}

	~timer() {

	}

};

_NSTD_END
#endif !_NSTD_TIMER_