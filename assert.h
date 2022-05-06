#pragma once
#ifndef _NSTD_ASSERT
#include "Defines.h"

_NSTD_BEGIN
template <class _Ty>
class _NSTD_AHA {
private:
	_Ty* _arr;
public:
	_NSTD_AHA(int len) { 
		_arr = new _Ty[len]; 
	}
	~_NSTD_AHA() { 
		delete[] _arr;       
		_arr = NULL;
	}

	_Ty*& get_arr() { 
		return _arr;         
	}
	_Ty& operator[] (int i) { 
		return _arr[i];      
	}
};
int c_str_len(const char*& c_str) {
	int out = 0;
	for (char* ptr = const_cast<char*> (c_str); ptr != '\0'; ptr++)
		out++;
	return out;
}
int int_list_sum(int* arr, int len) {
	int out = 0;
	_NSTD_FOR(len)
		out += arr[i];
	return out;
}
template <typename _Ty>
void copy_to_out(_NSTD _NSTD_AHA<_Ty>& s_arr, const _Ty* arr, const int len, int& cashe = 0) {
	for (int i = 0; i < len; i++)
		s_arr[cashe + i] = arr[i];
	cashe += len;
}
_NSTD_END

_NSTD _NSTD_AHA<char>& assert_str(char*, char*, int, char*);

// "ERROR: msg | FROM: #expr | ON LINE: __LINE__ | IN FILE: __FILE__ ";
#define _NSTD_ASSERT(expr, msg) static_assert(!!(expr), assert_str(msg, #expr, __LINE__, __FILE__).get_arr())
#endif // ifndef _NSTD_ASSERT