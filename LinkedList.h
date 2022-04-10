#pragma once

#include <assert.h>
#include <memory>

template <typename _Ty>
struct _Linked_item {
	_Linked_item(_Ty& val, _Linked_item* next = nullptr, _Linked_item* prev = nullptr) 
	:	val_(val), next_(next), prev_(prev) {}
	
	_Ty val_; _Linked_item* next_, prev_;
};

template <typename _Ty, class _Alloc = std::allocator<_Ty>>
class _Linked_list {
private:
	using Item = _Linked_item<_Ty>;
	//using traits_t = std::allocator_traits<decltype(_Alloc)>;
public:
	_Linked_list() : first_item_(nullptr), last_item_(nullptr) {}
	_Linked_list(_Ty i) : first_item_(new_plus_assert(i, nullptr, nullptr)), last_item_(nullptr) {}
	_Linked_list(_Linked_list& d) : first_item_(d.front_ptr()), last_item_(d.back_ptr()) {}
	~_Linked_list() {
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
	}
	Item* front_ptr() { return first_item_; }
	Item* back_ptr () { return  last_item_; }

	void emplace_front(_Ty& i) {
		if (first_item_ == nullptr) {
			if (last_item_ != nullptr) {
				Item* cashe = last_item_;
				while (cashe->prev_ != nullptr) cashe = cashe->prev_;
				first_item_ = new_plus_assert(i, cashe, nullptr);
				cashe->prev_ = first_item_;
			}
			else {
				first_item_ = new_plus_assert(i, nullptr, nullptr);
			}
		}
		else {
			first_item_->prev_ = new_plus_assert(i, first_item_, nullptr);
			first_item_ = first_item_->prev_;
		}
	}
	void emplace_back(_Ty& i) {
		if (last_item_ == nullptr) {
			if (first_item_ != nullptr) {
				Item* cashe = first_item_;
				while (cashe->next_ != nullptr) cashe = cashe->next_;
				last_item_ = new_plus_assert(i, nullptr, first_item_);
				cashe->next_ = last_item_;
			}
			else {
				last_item_ = new_plus_assert(i, nullptr, nullptr);
			}
		}
		else {
			last_item_->next_ = new_plus_assert(i, nullptr, last_item_);
			last_item_ = last_item_->next_;
		}
	}
	void push_back (_Ty i) {  emplace_back(i); }
	void push_front(_Ty i) { emplace_front(i); }
	void push_array_front(_Ty* arr, _Ty* end) { for (_Ty* i = arr; i != end; i++) emplace_front(*i); }
	void push_array_back (_Ty* arr, _Ty* end) { for (_Ty* i = arr; i != end; i++)  emplace_back(*i); }
	void push_array_front(_Ty* arr,  int len) { push_array_front(arr, arr + len); }
	void push_array_back (_Ty* arr,  int len) { push_array_back (arr, arr + len); }

	_Ty front() {
		if (last_item_ != nullptr && first_item_ == nullptr && last_item_->prev_ == nullptr) {
			return last_item_->val_;
		}
		assert(pop_possible());
		return first_item_->val_;
	}
	_Ty pop_front() {
		if (last_item_ != nullptr && first_item_ == nullptr && last_item_->prev_ == nullptr) {
			_Ty out = last_item_->val_;
			last_item_ = nullptr;
			return out;
		}
		assert(pop_possible());
		Item* cashe = first_item_;
		first_item_ = first_item_->next_;
		_Ty out = cashe->val_;
		delete cashe;
		return out;
	}
	_Ty back() {
		if (first_item_ != nullptr && last_item_ == nullptr && first_item_->next_ == nullptr) {
			return first_item_->val_;
		}
		assert(pop_possible());
		return last_item_->val_;
	}
	_Ty pop_back() {
		if (first_item_ != nullptr && last_item_ == nullptr && first_item_->next_ == nullptr) {
			_Ty out = first_item_->val_;
			first_item_ = nullptr;
			return out;
		}
		assert(pop_possible());
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
	bool pop_possible() {
		if (first_item_ == nullptr && last_item_ == nullptr) return false;
		else if (first_item_ == nullptr) {
			Item* cashe = last_item_;
			while (cashe->prev_ != nullptr) cashe = cashe->prev_;
			first_item_ = cashe;
		}
		else if (last_item_ == nullptr) {
			Item* cashe = first_item_;
			while (cashe->next_ != nullptr) cashe = cashe->next_;
			last_item_ = cashe;
		}
		return true;
	}

	Item* first_item_, last_item_;
};