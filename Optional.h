#pragma once
#ifndef _CUSTOM_OPTIONAL_
#define _CUSTOM_OPTIONAL_

#include <assert.h>

template <typename _Ty>
class _Optional {
	_Optional() : obj_ptr(NULL) {}
	_Optional(_Ty obj) { set_obj(obj); }
	~_Optional() { delete obj_ptr; }

	_Ty get_obj() {
		assert(is_set());
		return *obj_ptr;
	}
	void set_obj(_Ty obj) {
		if (obj_ptr) delete obj_ptr;
		obj_ptr = new _Ty;
		*obj_ptr = obj;
	}
	_Ty pop() {
		assert(is_set());
		_Ty cashe = *obj_ptr
		delete obj_ptr;
		obj_ptr = NULL;
		return cashe;
	}
	inline bool is_set() {
		return static_cast<bool> (obj_ptr);
	}
private:
	_Ty* obj_ptr;
};

#endif // ifndef _CUSTOM_OPTIONAL_