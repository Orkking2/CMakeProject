#pragma once
#ifndef _NSTD_OPTIONAL_
#define _NSTD_OPTIONAL_

#include "Defines.h"
#include "assert.h"

_NSTD_BEGIN
template <typename _Ty>
class _Optional {
public:
	_Optional() : _obj_ptr(NULL) {}
	_Optional(_Ty obj) { set(obj); }
	~_Optional() { delete _obj_ptr; }
	_Ty& get() {
		_NSTD_ASSERT(is_set(), _not_set_err);
		return *_obj_ptr;
	}
	void set(_Ty obj) {
		if (_obj_ptr) delete _obj_ptr;
		_obj_ptr = new _Ty;
		*_obj_ptr = obj;
	}
	_Ty pop() {
		_NSTD_ASSERT(is_set(), _not_set_err);
		_Ty cashe = *_obj_ptr
		delete _obj_ptr;
		_obj_ptr = NULL;
		return cashe;
	}
	bool is_set() {
		return static_cast<bool> (_obj_ptr);
	}
private:
	_Ty* _obj_ptr;
	static const char* _not_set_err = "Trying to access unset optional";
};


_NSTD_END
#endif // ifndef _NSTD_OPTIONAL_