#pragma once
#ifndef _NSTD_OPTIONAL_
#define _NSTD_OPTIONAL_

#include "assert.h"

_NSTD_BEGIN
template <typename _Ty>
class _Optional {
private:
	static const char* _not_set_err = "Trying to access unset optional";
	_Ty* _obj_ptr;
public:
	_Optional() : _obj_ptr(NULL) {}
	_Optional(_Ty obj) { 
		set(obj); 
	}
	~_Optional() { 
		unset();
	}

	_Ty& get() {
		_NSTD_ASSERT(is_set(), _not_set_err);
		return *_obj_ptr;
	}

	_Ty pop() {
		_NSTD_ASSERT(is_set(), _not_set_err);
		_Ty cashe = *_obj_ptr;
		unset();
		return cashe;
	}

	void set(_Ty obj) {
		if (_obj_ptr) delete _obj_ptr;
		_obj_ptr = new _Ty;
		*_obj_ptr = obj;
	}
	void unset() {
		delete _obj_ptr;
		_obj_ptr = NULL;
	}

	bool is_set() {
		return (_obj_ptr != NULL);
	}

};


_NSTD_END
#endif // ifndef _NSTD_OPTIONAL_