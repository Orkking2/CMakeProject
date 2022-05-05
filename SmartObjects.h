#pragma once
#ifndef _NSTD_SMART_OBJECTS_
#define _NSTD_SMART_OBJECTS_

#include "Defines.h"

_NSTD_BEGIN
template<class _Ty>
class _S_OBJ {
private:
	_Ty* _val;
public:
	_S_OBJ(_Ty in) {
		_val = new _Ty(in);
	}
	_Ty get() {
		return *_val;
	}
	~_S_OBJ() {
		delete _val;
	}
};

template <class _Ty>
class _S_ARR {
private:
	_Ty* _arr;
public:
	_S_ARR(_Ty* arr, int len) {
		_arr = new _Ty[len];
		for (int i = 0; i < len; i++)
			_arr[i] = arr[i];
	}
	_S_ARR(int len) {
		_arr = new _Ty[len];
	}

	_Ty*& get_arr() {
		return _arr;
	}
	
	_Ty& operator[] (int i) {
		return _arr[i];
	}

	~_S_ARR() {
		delete[] _arr;
	}
};

_NSTD_END
#endif // ifndef _NSTD_SMART_OBJECTS_