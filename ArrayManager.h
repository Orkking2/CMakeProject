#pragma once
#ifndef _NSTD_ARRAY_MANAGER_
#define _NSTD_ARRAY_MANAGER_

#include "Assert.h"

_NSTD_BEGIN
template <typename _Ty>
class vector {
private:
	_Ty* arr_;
	int len_;
public:
	vector(_Ty* arr, int len) : len_(len) {
		arr_ = new _Ty[len];
		for (int i = 0; i < len; i++)
			arr_[i] = arr[i];
	}
	vector(int len) {
		arr_ = new _Ty[len];
		len_ = len;
	}
	vector() : arr_(NULL), len_(0) {}
	~vector() { 
		sel_destruct(); 
	}
	void sel_destruct()  { 
		delete[] arr_; 
	}

	vector& operator = (const vector<_Ty>& arr) {
		sel_destruct();
		len_ = arr.get_len();
		arr_ = new _Ty[len_];
		for (int i = 0; i < len_; i++) arr_[i] = arr[i];
		return *this;
	}

	vector& emplace_back(_Ty& val) {
		_Ty* cashe = arr_;
		arr_ = new _Ty[len_ + 1];
		_NSTD_FOR(len_)
			arr_[i] = cashe[i];
		delete[] cashe;
		arr_[++len_] = val;
	}

	vector& push_back(_Ty val) {
		emplace_back(val);
	}

	vector& emplace_front(_Ty& val) {
		_Ty* cashe = arr_;
		arr_ = new _Ty
	}
	
	_NODISCARD _Ty*& get_arr() const { 
		return arr_; 
	}
		
	_NODISCARD int get_len() const { 
		return len_; 
	}

	void set_to_arr(_Ty* arr, int count) {
		sel_destruct();
		len_ = count;
		arr_ = new _Ty[count];
		for (int i = 0; i < count; i++) arr_[i] = arr[i];
	}

	_NODISCARD _Ty& operator [] (const int& i) {
		_NSTD_ASSERT(i < len_, "Attempted to access item outside arr size");
		return arr_[i];
	}

	vector& operator + (const vector<_Ty>& apc) {
		int apc_len = apc.get_len();
		_Ty* n_arr = npa(len_ += apc_len);
		for (int i = 0; i < apc_len; i++) n_arr[len_ + i] = apc[i];
		delete[] arr_;
		arr_ = n_arr;
		return *this;
	}

private:
	_Ty*& npa(int count) {
		_Ty* cashe = new _Ty[count];
		_NSTD_ASSERT(cashe, "Out of memory");
		return cashe;
	}

#ifdef _IOSTREAM_
public:
	template <typename _Ty>
	friend std::ostream& operator << (std::ostream& os, const vector<_Ty>& arr);
#endif // ifdef _IOSTREAM_
};

#ifdef _IOSTREAM_
template <typename _Ty>
std::ostream& operator << (std::ostream& os, const vector<_Ty>& arr) {
	for (int i = 0; i < arr.len_; i++) os << arr.arr_[i] << ", ";
	return os;
}
#endif // ifdef _IOSTREAM_


_NSTD_END
#endif // ifndef _ARRAY_MANAGER_