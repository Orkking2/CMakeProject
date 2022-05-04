#pragma once
#ifndef _NSTD_COPY_
#define _NSTD_COPY_

#include "Defines.h"

_NSTD_BEGIN
template<class _Ty>
class Copy {
private:
	_Ty* _val;
public:
	Copy(_Ty in) {
		_val = new _Ty(in);
	}
	_Ty get_copy() {
		return *_val;
	}
	~Copy() {
		delete _val;
	}
};

template <class _Ty>
class CopyArr {
private:
	_Ty* _arr;
public:
	CopyArr(_Ty* arr, int len) {
		_arr = new _Ty[len];
		for (int i = 0; i < len; i++)
			_arr[i] = arr[i];
	}
	_Ty*& get_arr() {
		return _arr;
	}
	~CopyArr() {
		delete[] _arr;
	}
};

_NSTD_END
#endif // ifndef _NSTD_COPY_