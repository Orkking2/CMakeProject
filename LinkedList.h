#pragma once

#include <assert.h>
#include <memory>
#include <cstdarg>

template <typename _Ty>
class _Linked_item {
private: 
	using Item = _Linked_item<_Ty>;
public:
	Item(const _Ty& val, const Item*& next = nullptr, const Item*& prev = nullptr) : val_(val), next_(next), prev_(prev) {}
	
	_Ty val_;
	Item* next_;
	Item* prev_;
};

template <typename _Ty>
class _Linked_list {
private:
	using Item = _Linked_item<_Ty>;
public:
	_Linked_list()                : first_item_(nullptr),                              last_item_(nullptr)      {}
	_Linked_list(_Ty i)           : first_item_(new_plus_assert(i, nullptr, nullptr)), last_item_(nullptr)      {}
	_Linked_list(_Linked_list& d) : first_item_(d.front_ptr()),                        last_item_(d.back_ptr()) {}
	// Ellipses are sussy
	_Linked_list(int count, ...)  : first_item_(nullptr),                              last_item_(nullptr)      {
		std::va_list list;
		va_start(list, count);
		for (int i = 0; i < count; i++) push_back(va_arg(list, _Ty));
		link();
		va_end(list);
	}
	~_Linked_list() {
		link();
		while (first_item_ != nullptr) {
			Item* cashe = first_item_;
			first_item_ = first_item_->next_;
			delete cashe;
		}
		if (last_item_ != nullptr) delete last_item_;
	}
	Item* front_ptr() { link(); return first_item_; }
	Item* back_ptr () { link(); return  last_item_; }

	void emplace_front(_Ty& i) {
		if (first_item_ == nullptr) { 
			link(); 
			first_item_ = new_plus_assert(i, nullptr, nullptr); }
		else {
			first_item_->prev_ = new_plus_assert(i, first_item_, nullptr);
			first_item_ = first_item_->prev_;
		}
	}
	void emplace_back(_Ty& i) {
		if (last_item_ == nullptr) { 
			link(); 
			last_item_ = new_plus_assert(i, nullptr, nullptr); }
		else {
			last_item_->next_ = new_plus_assert(i, nullptr, last_item_);
			last_item_ = last_item_->next_;
		}
	}
	inline void push_front(_Ty i) { emplace_front(i); }
	inline void push_back (_Ty i) { emplace_back (i); }

	inline void emplace_array_front(_Ty* arr, _Ty* end) { for (_Ty* i = arr; i != end; i++) emplace_front   (*i ); }
	inline void emplace_array_back (_Ty* arr, _Ty* end) { for (_Ty* i = arr; i != end; i++) emplace_back    (*i ); }
	inline void push_array_front   (_Ty* arr, _Ty* end) { for (_Ty* i = arr; i != end; i++) push_front      (*i ); }
	inline void push_array_back    (_Ty* arr, _Ty* end) { for (_Ty* i = arr; i != end; i++) push_back       (*i ); }
	inline void emplace_array_front(_Ty* arr, int  len) { for (int i = 0;    i <= len; i++) emplace_front(arr[i]); }
	inline void emplace_array_back (_Ty* arr, int  len) { for (int i = 0;    i <= len; i++) emplace_back (arr[i]); }
	inline void push_array_front   (_Ty* arr, int  len) { for (int i = 0;    i <= len; i++) push_front   (arr[i]); }
	inline void push_array_back    (_Ty* arr, int  len) { for (int i = 0;    i <= len; i++) push_back    (arr[i]); }

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
		while (first_item_ != nullptr) {
			Item* cashe = first_item_;
			first_item_ = first_item_->next_;
			delete cashe;
		}
		while (last_item_ != nullptr) {
			Item* cashe = last_item_;
			last_item_ = last_item_->prev_;
			delete cashe;
		}
		for (Item* i = d.back_ptr(); i != nullptr; i = i->prev_) {
			push_back(i);
		}
	}
private:
	Item* new_plus_assert(const _Ty& i, const Item*& next, const Item*& prev) {
		Item* ptr = new Item(i, next, prev);
		assert(ptr);
		return ptr;
	}
	void link() {
		if (first_item_ == nullptr && last_item_ == nullptr) {
			return;
		}
		else if (first_item_ != nullptr && first_item_->next_ == nullptr && last_item_ != nullptr) {
			for (Item* ptr = last_item_; ptr != nullptr; ptr = ptr->prev_) {
				first_item_->next_ = ptr;
			}
			link();
		}
		else if (last_item_ != nullptr && last_item_->prev_ == nullptr && first_item_ != nullptr) {
			for (Item* ptr = first_item_; ptr != nullptr; ptr = ptr->next_) {
				last_item_->prev_ = ptr;
			}
		}
		else if (first_item_ != nullptr && first_item_->next_ != nullptr && last_item_ == nullptr) {
			for (Item* ptr = first_item_; ptr != nullptr; ptr = ptr->next_) {
				last_item_ = ptr;
			}
		}
		else if (last_item_ != nullptr && last_item_->prev_ != nullptr && first_item_ == nullptr) {
			for (Item* ptr = first_item_; ptr != nullptr; ptr = ptr->next_) {
				last_item_ = ptr;
			}
		}
	}
	bool not_null() {
		if (first_item_ == nullptr && last_item_ == nullptr) { return false; }
		else { return true; }
	}

	Item* first_item_;
	Item* last_item_;
};