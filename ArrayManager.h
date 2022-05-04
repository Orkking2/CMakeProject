#pragma once
#ifndef _ARRAY_MANAGEMENT_
#define _ARRAY_MANAGEMENT_

#include "assert.h"

template <typename _Ty>
class _ARRAY_PLUS_COUNT {
private:
	_Ty* _arr;
	int _count;
public:
	_ARRAY_PLUS_COUNT(_Ty* arr = NULL, int count = 0) : _arr(arr), _count(count) {}
	~_ARRAY_PLUS_COUNT() { sel_destruct(); }
	void sel_destruct()  { delete[]  _arr; }
	_ARRAY_PLUS_COUNT<_Ty>& operator = (const _ARRAY_PLUS_COUNT<_Ty>& arr) {
		sel_destruct();
		_count = arr.get_count();
		_arr = new _Ty[_count];
		for (int i = 0; i < _count; i++) _arr[i] = arr[i];
		return *this;
	}
	_Ty*& get_arr() const { return _arr; }
	void set_to_arr(_Ty* arr, int count) {
		sel_destruct();
		_count = count;
		_arr = new _Ty[count];
		for (int i = 0; i < count; i++) _arr[i] = arr[i];
	}
	int get_count() const { return _count; }

	_Ty& operator [] (const int& i) {
		_ASSERT(i < _count, "Attempted to access item outside _ARRAY_PLUS_COUNT");
		return _arr[i];
	}

	_ARRAY_PLUS_COUNT& operator + (const _ARRAY_PLUS_COUNT<_Ty>& apc) {
		int apc_count = apc.get_count();
		_Ty* n_arr = npa(_count += apc_count);
		for (int i = 0; i < apc_count; i++) n_arr[_count + i] = apc[i];
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
	for (int i = 0; i < arr._count; i++) os << arr._arr[i] << ", ";
	return os;
}
#endif // ifdef _IOSTREAM_




#endif // ifndef _ARRAY_MANAGEMENT_