#pragma once
#ifndef _NSTD_LINKED_LIST_
#define _NSTD_LINKED_LIST_

#include "Defines.h"
#include <memory>
#include "ArrayManager.h" 

_NSTD_BEGIN
template <typename _Ty>
struct _LINKED_OBJECT {
	_LINKED_OBJECT(const _Ty& val, _LINKED_OBJECT* next, _LINKED_OBJECT* prev) : val_(val), next_(next), prev_(prev) {}

	_Ty val_;
	_LINKED_OBJECT* next_, * prev_;

	bool operator == (const _LINKED_OBJECT<_Ty>& i) {
		return (val_ == i.val_ && next_ == i.next_ && prev_ == i.prev_);
	}
};

template <typename _Ty>
class _LINKED_ARRAY {
private:
	_LINKED_OBJECT<_Ty>* first_item_, * last_item_;
public:
	_LINKED_ARRAY()                            : first_item_(NULL),   last_item_(NULL) {}
	_LINKED_ARRAY(_Ty i)                       : first_item_(npa(i)), last_item_(NULL) {}
	_LINKED_ARRAY(const _LINKED_ARRAY<_Ty>& l) : first_item_(NULL),   last_item_(NULL) { set_to_arr(l.get_array()); }
#ifdef _CSTDARG_ // Elipses are susge
	_LINKED_ARRAY(_Ty end, _Ty item, ...) : first_item_(NULL),               last_item_(NULL) {
		std::va_list list;
		va_start(list, item);
		for (_Ty i = item; i != end; i = va_arg(list, _Ty)) push_back(i);
		link();
		va_end(list);
	}
#endif // ifdef _CSTDARG_
	~_LINKED_ARRAY() { 
		destruct(); 
	}

	bool is_empty() const { 
		return !(first_item_ || last_item_); 
	}

	_LINKED_OBJECT<_Ty>* front_ptr() const { 
		return first_item_; 
	}
	_LINKED_OBJECT<_Ty>* back_ptr () const { 
		return last_item_;  
	}

	_NSTD vector<_Ty> get_array() const {
		int count = 0;
		for (_LINKED_OBJECT<_Ty>* ptr = first_item_; ptr; ptr = ptr->next_) count++;
		_LINKED_OBJECT<_Ty>* ptr = first_item_;
		_Ty* arr = new _Ty[count];
		for (int i = 0; i < count; i++) {
			arr[i] = ptr->val_;
			ptr = ptr->next_;
		}
		return _NSTD vector<_Ty>(arr, count);
	}
	void set_to_arr(_NSTD vector<_Ty> arr) { 
		destruct();
		_Ty* array = arr.get_arr();
		_Ty term = array[arr.get_len() - 1];
		push_array_back(array, term); 
	}

	void emplace_front(_Ty& i) {
		_LINKED_OBJECT<_Ty>* cashe = npa(i, first_item_, NULL);
		if (!last_item_)
			last_item_ = cashe;
		else
			first_item_->prev_ = cashe;
		first_item_ = cashe;
	}
	void emplace_back(_Ty& i) {
		_LINKED_OBJECT<_Ty>* cashe = npa(i, NULL, last_item_);
		if (!first_item_)
			first_item_ = cashe;
		else
			last_item_->next_ = cashe;
		last_item_ = cashe;
	}
	inline void push_front(_Ty i) { emplace_front(i); }
	inline void push_back (_Ty i) { emplace_back (i); }

	inline void emplace_array_front(_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) emplace_front(*i); }
	inline void emplace_array_back (_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) emplace_back (*i); }
	inline void push_array_front   (_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) push_front   (*i); }
	inline void push_array_back    (_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) push_back    (*i); }
	inline void emplace_array_front(_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) emplace_front(*i); }
	inline void emplace_array_back (_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) emplace_back (*i); }
	inline void push_array_front   (_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) push_front   (*i); }
	inline void push_array_back    (_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) push_back    (*i); }
#ifdef _LINKED_LIST_IS_NOT_USED_AS_INT_
	inline void emplace_array_front(_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) emplace_front(arr[i]); }
	inline void emplace_array_back (_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) emplace_back (arr[i]); }
	inline void push_array_front   (_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) push_front   (arr[i]); }
	inline void push_array_back    (_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) push_back    (arr[i]); }
#endif // _LINKED_LIST_IS_NOT_INT_

	_Ty get_front() {
		assert(not_null());
		return first_item_->val_;
	}
	void remove_front() {
		if (first_item_ == last_item_) {
			last_item_ = NULL;
			delete first_item_;
			first_item_ = NULL;
		}
		else {
			first_item_ = first_item_->next_;
			delete first_item_->prev_;
			first_item_->prev_ = NULL;
		}
	}
	_Ty pop_front() {
		assert(not_null());
		_Ty cashe = first_item_->val_;
		remove_front();
		return cashe;
	}
	_Ty get_back() {
		assert(not_null());
		return first_item_->val_;
	}
	void remove_back() {
		if (last_item_ == first_item_) {
			first_item_ = NULL;
			delete last_item_;
			last_item_ = NULL;
		}
		else {
			last_item_ = last_item_->prev_;
			delete last_item_->next_;
			last_item_->next_ = NULL;
		}
	}
	_Ty pop_back() {
		assert(not_null());
		_Ty cashe = last_item_->val_;
		remove_back();
		return cashe;
	}
	void operator = (const _LINKED_ARRAY& d) {
		destruct();
		for (_LINKED_OBJECT<_Ty>* i = d.back_ptr(); i; i = i->prev_) emplace_back(*i);
	}
#ifdef _IOSTREAM_
	template <typename _Ty>
	friend std::ostream& operator << (std::ostream& os, const _LINKED_ARRAY<_Ty>& list);
#endif // ifdef _IOSTREAM_
private:
	_LINKED_OBJECT<_Ty>* npa(const _Ty& i, _LINKED_OBJECT<_Ty>* next = NULL, _LINKED_OBJECT<_Ty>* prev = NULL) {
		_LINKED_OBJECT<_Ty>* ptr = new _LINKED_OBJECT<_Ty>(i, next, prev);
		assert(ptr);
		return ptr;
	}
	bool not_null() { return first_item_ || last_item_; }

	_LINKED_OBJECT<_Ty>* find_first() { 
		_LINKED_OBJECT<_Ty>* out; 
		for (_LINKED_OBJECT<_Ty>* ptr = last_item_;  ptr; ptr = ptr->prev_) 
			out = ptr; 
		return out; 
	}
	_LINKED_OBJECT<_Ty>* find_last () { 
		_LINKED_OBJECT<_Ty>* out; 
		for (_LINKED_OBJECT<_Ty>* ptr = first_item_; ptr; ptr = ptr->next_) 
			out = ptr; 
		return out; 
	}

public:
	void destruct() {
		last_item_ = NULL;
		while (first_item_) {
			if (first_item_->next_) {
				first_item_ = first_item_->next_;
				delete first_item_->prev_;
				first_item_->prev_ = NULL;
			}
			else { 
				delete first_item_; 
				first_item_ = NULL; 
			}
		}
	}
};

#ifdef _IOSTREAM_
template <typename _Ty>
std::ostream& operator << (std::ostream& os, const _LINKED_ARRAY<_Ty>& list) {
	return (os << list.get_array());
}
#endif // ifdef _IOSTREAM_
_NSTD_END
#endif // ifndef _NSTD_LINKED_LIST_