#pragma once

#include <assert.h>
#include <memory>

template <typename _Ty>
struct _Linked_item {
	_Linked_item(_Ty& val, _Linked_item* next = nullptr, _Linked_item* prev = nullptr) : val_(val), next_(next), prev_(prev) {}
	
	_Ty val_; _Linked_item* next_, prev_;

	inline bool operator == (const _Linked_item& l) {
		return val_ == l.val_ && next_ == l.next_ && prev_ == l.prev_;
	}
	inline bool operator != (const _Linked_item& l) {
		return !(this == l);
	}
};

template <typename _Ty>
class _Linked_list {
private:
	using Item = _Linked_item<_Ty>;
public:
	_Linked_list()                : first_item_(nullptr),                              last_item_(nullptr)      {}
	_Linked_list(_Ty i)           : first_item_(new_plus_assert(i, nullptr, nullptr)), last_item_(nullptr)      {}
	_Linked_list(_Linked_list& d) : first_item_(d.front_ptr()),                        last_item_(d.back_ptr()) {}
	~_Linked_list() {
		link;
		while (first_item_ != nullptr) {
			Item* cashe = first_item_;
			first_item_ = first_item_->next_;
			delete cashe;
		}
	}
	Item* front_ptr() { return first_item_; }
	Item* back_ptr () { return  last_item_; }

	void emplace_front(_Ty& i) {
		if (first_item_ == nullptr) link;
		if (first_item_ == nullptr) first_item_ = new_plus_assert(i, nullptr, nullptr);
		else {
			first_item_->prev_ = new_plus_assert(i, first_item_, nullptr);
			first_item_ = first_item_->prev_;
		}
	}
	void emplace_back(_Ty& i) {
		if (last_item_ == nullptr) link;
		if (last_item_ == nullptr) last_item_ = new_plus_assert(i, nullptr, nullptr);
		else {
			last_item_->next_ = new_plus_assert(i, nullptr, last_item_);
			last_item_ = last_item_->next_;
		}
	}
	inline void push_front(_Ty i) { emplace_front(i); }
	inline void push_back (_Ty i) { emplace_back (i); }

	void emplace_array_front       (const _Ty*& arr, const _Ty*& end) { for (_Ty* i = arr; i != end; i++) emplace_front(*i); }
	void emplace_array_back        (const _Ty*& arr, const _Ty*& end) { for (_Ty* i = arr; i != end; i++) emplace_back (*i); }
	void push_array_front          (const _Ty*& arr, const _Ty*& end) { for (_Ty* i = arr; i != end; i++) push_front   (*i); }
	void push_array_back           (const _Ty*& arr, const _Ty*& end) { for (_Ty* i = arr; i != end; i++) push_back    (*i); }
	inline void emplace_array_front(const _Ty*& arr, const  int& len) { emplace_array_front(arr, arr + len); }
	inline void emplace_array_back (const _Ty*& arr, const  int& len) { emplace_array_back (arr, arr + len); }
	inline void push_array_front   (const _Ty*& arr, const  int& len) { push_array_front   (arr, arr + len); }
	inline void push_array_back    (const _Ty*& arr, const  int& len) { push_array_back    (arr, arr + len); }

	_Ty front() {
		assert(pop_possible());
		link;
		return first_item_->val_;
	}
	_Ty pop_front() {
		assert(pop_possible());
		link;
		Item* cashe = first_item_;
		first_item_ = first_item_->next_;
		_Ty out = cashe->val_;
		delete cashe;
		return out;
	}
	_Ty back() {
		assert(pop_possible());
		link;
		return last_item_->val_;
	}
	_Ty pop_back() {
		assert(pop_possible());
		link;
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
	Item* new_plus_assert(_Ty& i, Item* next, Item* prev) {
		Item* ptr = new Item(_Ty, next, prev);
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
			if (last_item_->prev_ == nullptr) link();
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
	bool pop_possible() {
		if (first_item_ == nullptr && last_item_ == nullptr) return false;
		else return true;
	}

	Item* first_item_, last_item_;
};