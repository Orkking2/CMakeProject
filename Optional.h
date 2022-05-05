#pragma once
#ifndef _CUSTOM_OPTIONAL_
#define _CUSTOM_OPTIONAL_

#include "Defines.h"
#include "assert.h"

#define _OPTIONAL_NOT_SET "Trying to access unset optional"

_NSTD_BEGIN
template <typename _Ty>
class _Optional {
public:
	_Optional() : _obj_ptr(NULL) {}
	_Optional(_Ty obj) { set_obj(obj); }
	~_Optional() { delete _obj_ptr; }
	_Ty get_obj() {
		_NSTD_ASSERT(is_set(), _OPTIONAL_NOT_SET);
		return *_obj_ptr;
	}
	void set_obj(_Ty obj) {
		if (_obj_ptr) delete _obj_ptr;
		_obj_ptr = new _Ty;
		*_obj_ptr = obj;
	}
	_Ty pop() {
		_NSTD_ASSERT(is_set(), _OPTIONAL_NOT_SET);
		_Ty cashe = *_obj_ptr
		delete obj_ptr;
		_obj_ptr = NULL;
		return cashe;
	}
	inline bool is_set() {
		return static_cast<bool> (_obj_ptr);
	}
private:
	_Ty* _obj_ptr;
};


_NSTD_END
#endif // ifndef _CUSTOM_OPTIONAL_