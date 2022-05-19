#pragma once
#ifndef _NSTD_SORT_
#define _NSTD_SORT_

#include "Threads.h"

_NSTD_BEGIN
template <typename _Ty>
struct item {
	item(_Ty val) : val_(val) {}

	_Ty val_;
	bool operator < (const item& r) {
		return val_ < r.val_
	}
};

_NSTD_END
#endif