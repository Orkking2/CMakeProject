#pragma once

#include <assert.h>
#include <cstdarg>
#include <memory>

#ifndef NULL
#define NULL 0
#endif // ifndef NULL


template <typename _Ty>
class _Linked_item {
private: 
	using Item = _Linked_item<_Ty>;
public:
	Item(const _Ty& val, Item* next = NULL, Item* prev = NULL) : val_(val), next_(next), prev_(prev) {}
	
	_Ty val_;
	Item* next_;
	Item* prev_;
};

template <typename _Ty>
class _Linked_list {
private:
	using Item = _Linked_item<_Ty>;

	Item* first_item_;
	Item* last_item_;
public:
	_Linked_list()                : first_item_(NULL),                           last_item_(NULL)         {}
	_Linked_list(_Ty i)           : first_item_(new_plus_assert(i, NULL, NULL)), last_item_(NULL)         {}
	_Linked_list(_Linked_list& l) : first_item_(l.front_ptr()),                  last_item_(l.back_ptr()) {}
	// Ellipses are sussy
	_Linked_list(int count, ...)  : first_item_(NULL),                           last_item_(NULL)         {
		std::va_list list;
		va_start(list, count);
		for (int i = 0; i < count; i++) push_back(va_arg(list, _Ty));
		link();
		va_end(list);
	}
	~_Linked_list() {
		link();
		while (first_item_) {
			Item* cashe = first_item_;
			first_item_ = first_item_->next_;
			delete cashe;
		}
		if (last_item_) delete last_item_;
	}

	Item* front_ptr() { link(); return first_item_; }
	Item* back_ptr () { link(); return last_item_;  }

	void emplace_front(_Ty& i) {
		if (!first_item_) { first_item_ = new_plus_assert(i, NULL, NULL); link(); }
		else {
			first_item_->prev_ = new_plus_assert(i, first_item_, NULL);
			first_item_ = first_item_->prev_;
		}
	}
	void emplace_back(_Ty& i) {
		if (!last_item_) { last_item_ = new_plus_assert(i, NULL, NULL); link(); }
		else {
			last_item_->next_ = new_plus_assert(i, NULL, last_item_);
			last_item_ = last_item_->next_;
		}
	}
	inline void push_front(_Ty i) { emplace_front(i); }
	inline void push_back (_Ty i) { emplace_back (i); }

	inline void emplace_array_front(_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) emplace_front   (*i ); }
	inline void emplace_array_back (_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) emplace_back    (*i ); }
	inline void push_array_front   (_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) push_front      (*i ); }
	inline void push_array_back    (_Ty* arr, _Ty* end) { for (_Ty* i = arr;  i != end; i++) push_back       (*i ); }
	inline void emplace_array_front(_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) emplace_front   (*i ); }
	inline void emplace_array_back (_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) emplace_back    (*i ); }
	inline void push_array_front   (_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) push_front      (*i ); }
	inline void push_array_back    (_Ty* arr, _Ty  end) { for (_Ty* i = arr; *i != end; i++) push_back       (*i ); }
	/*
	inline void emplace_array_front(_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) emplace_front(arr[i]); }
	inline void emplace_array_back (_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) emplace_back (arr[i]); }
	inline void push_array_front   (_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) push_front   (arr[i]); }
	inline void push_array_back    (_Ty* arr, int  len) { for (int  i = 0;    i  < len; i++) push_back    (arr[i]); }
	*/

	_Ty front() {
		assert(not_null());
		link();
		return first_item_->val_;
	}
	_Ty pop_front() {
		assert(not_null());
		link();
		Item* cashe = first_item_;
		first_item_ = first_item_->next_;
		_Ty out = cashe->val_;
		delete cashe;
		return out;
	}
	_Ty back() {
		assert(not_null());
		link();
		return last_item_->val_;
	}
	_Ty pop_back() {
		assert(not_null());
		link();
		Item* cashe = last_item_;
		last_item_ = last_item_->prev_;
		_Ty out = cashe->val_;
		delete cashe;
		return out;
	}
	void operator = (const _Linked_list& d) {
		link();
		while (first_item_) {
			Item* cashe = first_item_;
			first_item_ = first_item_->next_;
			delete cashe;
		}
		if (last_item_) delete last_item_;
		for (Item* i = d.back_ptr(); i; i = i->prev_) emplace_back(*i);
	}
private:
	Item* new_plus_assert(const _Ty& i, Item* next, Item* prev) {
		Item* ptr = new Item(i, next, prev);
		assert(ptr);
		return ptr;
	}
	bool not_null() { if (!first_item_ && !last_item_) return false; return true; }
	void link() {
		if      (!first_item_ && !last_item_) {}
		else if ( first_item_ && !first_item_->next_ &&  last_item_ ) { for (Item* ptr = last_item_;  ptr; ptr = ptr->prev_) first_item_->next_ = ptr; link(); }
		else if ( last_item_  && !last_item_ ->prev_ &&  first_item_) { for (Item* ptr = first_item_; ptr; ptr = ptr->next_) last_item_ ->prev_ = ptr; link(); }
		else if ( last_item_  &&  last_item_ ->prev_ && !first_item_) { for (Item* ptr = last_item_;  ptr; ptr = ptr->prev_) first_item_ = ptr; }
		else if ( first_item_ &&  first_item_->next_ && !last_item_ ) { for (Item* ptr = first_item_; ptr; ptr = ptr->next_) last_item_  = ptr; }
	}
};