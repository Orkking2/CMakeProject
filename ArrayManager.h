#pragma once
#ifndef _ARRAY_MANAGEMENT_
#define _ARRAY_MANAGEMENT_

#include "assert.h"
#include <assert.h>

template <typename _Ty>
class _ARRAY_PLUS_COUNT {
private:
	_Ty* arr_;
	int count_;
public:
	_ARRAY_PLUS_COUNT(_Ty* arr = NULL, int count = 0) : arr_(arr), count_(count) {}
	~_ARRAY_PLUS_COUNT() { sel_destruct(); }
	void sel_destruct()  { delete[]  arr_; }
	_ARRAY_PLUS_COUNT<_Ty>& operator = (const _ARRAY_PLUS_COUNT<_Ty>& arr) {
		sel_destruct();
		count_ = arr.get_count();
		arr_ = new _Ty[count_];
		for (int i = 0; i < count_; i++) arr_[i] = arr[i];
		return *this;
	}
	_Ty*& get_arr() const { return arr_; }
	void set_to_arr(_Ty* arr, int count) {
		sel_destruct();
		count_ = count;
		arr_ = new _Ty[count];
		for (int i = 0; i < count; i++) arr_[i] = arr[i];
	}
	int get_count() const { return count_; }

	_Ty& operator [] (const int& i) {
		_ASSERT_(i < count_, "Attempted to access item outside arr size");
		return arr_[i];
	}

	_ARRAY_PLUS_COUNT& operator + (const _ARRAY_PLUS_COUNT<_Ty>& apc) {
		int apc_count = apc.get_count();
		_Ty* n_arr = npa(count_ += apc_count);
		for (int i = 0; i < apc_count; i++) n_arr[count_ + i] = apc[i];
		delete[] arr_;
		arr_ = n_arr;
		return *this;
	}
private:
	_Ty*& npa(int count) {
		_Ty* cashe = new _Ty[count];
		_ASSERT_(cashe, "Out of memory");
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
	for (int i = 0; i < arr.count_; i++) os << arr.arr_[i] << ", ";
	return os;
}
#endif // ifdef _IOSTREAM_




#endif // ifndef _ARRAY_MANAGEMENT_