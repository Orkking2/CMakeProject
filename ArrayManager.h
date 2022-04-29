#pragma once
#ifndef _ARRAY_MANAGEMENT_
#define _ARRAY_MANAGEMENT_

#include <typeinfo>
#include "Assert.h"

template <typename _Ty>
struct _Array_with_count {
	_Array_with_count(_Ty* arr = NULL, int count = 0) : arr(arr), count(count) {}
	_Ty* arr;
	int count;
	~_Array_with_count() { delete[] arr; }
	void operator = (const _Array_with_count<_Ty>& array) {
		delete[] arr;
		count = array.count;
		arr = new _Ty[count];
		for (int i = 0; i < count; i++) arr[i] = array.arr[i];
	}
	_Ty& operator [] (const int& i) {
		return arr[i];
	}
	void operator + (const _Array_with_count<_Ty>& r) {
		_Ty* n_arr = new _Ty[count + r.count];
		for (int i = 0; i < r.count; i++) n_arr[count + i] = r.arr[i];
		delete[] arr;
		count += r.count;
		arr = n_arr;
	}
	void operator + (char* string) {
		_ASSERT_(typeid(_Ty) == typeid(char), "Invalid addition of string to _Array_with_count object of different type");

	}
#ifdef _IOSTREAM_
	template <typename _Ty>
	friend std::ostream& operator << (std::ostream& os, const _Array_with_count<_Ty>& arr);
#endif // ifdef _IOSTREAM_
};

#ifdef _IOSTREAM_
template <typename _Ty>
std::ostream& operator << (std::ostream& os, const _Array_with_count<_Ty>& arr) {
	for (int i = 0; i < arr.count; i++) os << arr.arr[i] << ", ";
	return os;
}
#endif // ifdef _IOSTREAM_




#endif // ifndef _ARRAY_MANAGEMENT_