#pragma once
#ifndef _NSTD_ARRAY_MANAGER_
#define _NSTD_ARRAY_MANAGER_

#include "assert.h"

_NSTD_BEGIN
template <typename _Ty>
class _ARRAY_PLUS_COUNT {
private:
	_Ty* _arr;
	int _len;
public:
	_ARRAY_PLUS_COUNT(_Ty* arr, int len) : _len(len) {
		_arr = new _Ty[len];
		for (int i = 0; i < len; i++)
			_arr[i] = arr[i];
	}
	_ARRAY_PLUS_COUNT(int len) {
		_arr = new _Ty[len];
		_len = len;
	}
	_ARRAY_PLUS_COUNT() : _arr(NULL), _len(0) {}
	~_ARRAY_PLUS_COUNT() { 
		sel_destruct(); 
	}
	void sel_destruct()  { 
		delete[] _arr; 
	}

	_ARRAY_PLUS_COUNT& operator = (const _ARRAY_PLUS_COUNT<_Ty>& arr) {
		sel_destruct();
		_len = arr.get_len();
		_arr = new _Ty[_len];
		for (int i = 0; i < _len; i++) _arr[i] = arr[i];
		return *this;
	}
	
	_Ty*& get_arr() const { 
		return _arr; 
	}
		
	int get_len() const { 
		return _len; 
	}

	void set_to_arr(_Ty* arr, int count) {
		sel_destruct();
		_len = count;
		_arr = new _Ty[count];
		for (int i = 0; i < count; i++) _arr[i] = arr[i];
	}

	_Ty& operator [] (const int& i) {
		_NSTD_ASSERT(i < _len, "Attempted to access item outside arr size");
		return _arr[i];
	}

	_ARRAY_PLUS_COUNT& operator + (const _ARRAY_PLUS_COUNT<_Ty>& apc) {
		int apc_len = apc.get_len();
		_Ty* n_arr = npa(_len += apc_len);
		for (int i = 0; i < apc_len; i++) n_arr[_len + i] = apc[i];
		delete[] _arr;
		_arr = n_arr;
		return *this;
	}

private:
	_Ty*& npa(int count) {
		_Ty* cashe = new _Ty[count];
		_ASSERT(cashe, "Out of memory");
		return cashe;
	}

#ifdef _IOSTREAM_
public:
	template <typename _Ty>
	friend std::ostream& operator << (std::ostream& os, const _ARRAY_PLUS_COUNT<_Ty>& arr);
#endif // ifdef _IOSTREAM_
};

#ifdef _IOSTREAM_
template <typename _Ty>
std::ostream& operator << (std::ostream& os, const _ARRAY_PLUS_COUNT<_Ty>& arr) {
	for (int i = 0; i < arr._len; i++) os << arr._arr[i] << ", ";
	return os;
}
#endif // ifdef _IOSTREAM_


_NSTD_END
#endif // ifndef _ARRAY_MANAGER_