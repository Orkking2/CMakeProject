#pragma once
#ifndef _NSTD_COPY_
#define _NSTD_COPY_

#include "Defines.h"

_NSTD_BEGIN
template<class _Ty>
class Copy {
private:
	_Ty* val;
public:
	Copy(_Ty in) {
		val = new _Ty(in);
	}
	_Ty get_copy() {
		return *val;
	}
	~Copy() {
		delete val;
	}
};

_NSTD_END
#endif // ifndef _NSTD_COPY_