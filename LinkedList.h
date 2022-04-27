#pragma once
#ifndef _LINKEDLIST_
#define _LINKEDLIST_

#define _CONST_COUNT_DISPLACE_ 1

#include <assert.h>
#include <memory>

#ifndef NULL
#define NULL 0
#endif // ifndef NULL

template <typename _Ty>
struct _Linked_item {
	_Linked_item(const _Ty& val, _Linked_item* next, _Linked_item* prev) : val_(val), next_(next), prev_(prev) {}

	_Ty val_;
	_Linked_item* next_;
	_Linked_item* prev_;

	bool operator == (const _Linked_item& i) {
		return (this->val_ == i.val_ && this->next_ == i.next_ && this->prev_ == i.prev_);
	}
};

template <typename _Ty>
struct _Array_with_count {
	_Array_with_count(_Ty* arr, int count) : arr(arr), count(count) {}
	_Ty* arr;
	int count;
	~_Array_with_count() { delete[] arr; }
#ifdef _IOSTREAM_
	template <typename _Ty>
	friend std::ostream& operator << (std::ostream& os, const _Array_with_count<_Ty>& arr);
}; // _Array_with_count

template <typename _Ty>
std::ostream& operator << (std::ostream& os, const _Array_with_count<_Ty>& arr) {
	for (int i = 0; i < arr.count; i++) os << arr.arr[i] << ", ";
	return os;
}
#else
}; // _Array_with_count
#endif // ifdef _IOSTREAM_

template <typename _Ty>
class _Linked_list {
private:
	using Array = _Array_with_count<_Ty>;
	using Item  = _Linked_item<_Ty>;

	Item* first_item_ = NULL;
	Item* last_item_  = NULL;
public:
	_Linked_list()                       : first_item_(NULL),               last_item_(NULL) {}
	_Linked_list(_Ty i)                  : first_item_(npa(i, NULL, NULL)), last_item_(NULL) {}
	_Linked_list(_Linked_list& l)        : first_item_(NULL),               last_item_(NULL) { set_to_arr(l.get_array()); }
#ifdef _CSTDARG_ // Elipses are susge
	_Linked_list(_Ty end, _Ty item, ...) : first_item_(NULL),               last_item_(NULL) {
		std::va_list list;
		va_start(list, item);
		for (_Ty i = item; i != end; i = va_arg(list, _Ty)) push_back(i);
		link();
		va_end(list);
	}
#endif // ifdef _CSTDARG_
	~_Linked_list() { 
		destruct(); 
	}

	bool is_empty() { return !(first_item_ || last_item_); }

	Item* front_ptr() { return first_item_; }
	Item* back_ptr () { return last_item_;  }

	Array get_array() {
		int count = 0;
		for (Item* ptr = first_item_; ptr; ptr = ptr->next_) count++;
		Item* ptr = first_item_;
		_Ty* arr = new _Ty[count];
		for (int i = 0; i < count; i++) {
			arr[i] = ptr->val_;
			ptr = ptr->next_;
		}
		return Array(arr, count);
	}
	void set_to_arr(Array arr) { destruct(); push_array_back(arr.arr, arr.arr[arr.count - _CONST_COUNT_DISPLACE_]); }

	void emplace_front(_Ty& i) {
		Item* cashe = npa(i, first_item_, NULL);
		if (!last_item_)
			last_item_ = cashe;
		else
			first_item_->prev_ = cashe;
		first_item_ = cashe;
	}
	void emplace_back(_Ty& i) {
		Item* cashe = npa(i, NULL, last_item_);
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
	void operator = (const _Linked_list& d) {
		destruct();
		for (Item* i = d.back_ptr(); i; i = i->prev_) emplace_back(*i);
	}
#ifdef _IOSTREAM_
	template <typename _Ty>
	friend std::ostream& operator << (std::ostream& os, const _Linked_list<_Ty>& list);
#endif // ifdef _IOSTREAM_
private:
	Item* npa(const _Ty& i, Item* next = NULL, Item* prev = NULL) {
		Item* ptr = new Item(i, next, prev);
		assert(ptr);
		return ptr;
	}
	bool not_null() { return first_item_ || last_item_; }

	Item* find_first() { Item* out; for (Item* ptr = last_item_;  ptr; ptr = ptr->prev_) out = ptr; return out; }
	Item* find_last () { Item* out; for (Item* ptr = first_item_; ptr; ptr = ptr->next_) out = ptr; return out; }

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
std::ostream& operator << (std::ostream& os, const _Linked_list<_Ty>& list) {
	return (os << list.get_array());
}
#endif // ifdef _IOSTREAM_

#endif // ifndef _LINKEDLIST_