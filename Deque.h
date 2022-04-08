#pragma once

#include <iostream>
#include <assert.h>

template <typename Type>
struct Item {
	Item(Type& val, Item* next = nullptr, Item* prev = nullptr) : val_(val), next_(next), prev_(prev) {}
	Type val_;
	Item* next_;
	Item* prev_;
};

template <typename Type>
class Deque {
public:
	using Item = Item<Type>;
	Deque() : first_item_(nullptr), last_item_(nullptr) {}
	Deque(Type& i) : first_item_(new_plus_assert(i, nullptr, nullptr)), last_item_(nullptr) {}
	Deque(Deque& d) : first_item_(d.front_ptr()), last_item_(d.back_ptr()) {}
	~Deque() {
		while (first_item_ != nullptr) {
			Item* cashe = first_item_;
			first_item_ = first_item_->next_;
			delete cashe;
		}
		while (last_item_ != nullptr) {
			Item* cashe = last_item_;
			last_item_ = last_item_->next_;
			delete cashe;
		}
	}
	Item* front_ptr() {
		return first_item_;
	}
	Item* back_ptr() {
		return last_item_;
	}

	void emplace_front(Type& i) {
		if (first_item_ == nullptr) {
			first_item_ = new_plus_assert(i, last_item_, nullptr);
			if (last_item_ != nullptr) {
				Item* cashe = last_item_;
				while (cashe->prev_ != nullptr) cashe = cashe->prev_;
				cashe->prev_ = first_item_;
			}
		}
		else {
			first_item_->prev_ = new_plus_assert(i, first_item_, nullptr);
			first_item_ = first_item_->prev_;
		}
	}
	inline void push_back(Type i) {
		emplace_back(i);
	}
	void emplace_back(Type& i) {
		if (last_item_ == nullptr) {
			last_item_ = new_plus_assert(i, nullptr, first_item_);
			if (first_item_ != nullptr) {
				Item* cashe = first_item_;
				while (cashe->next_ != nullptr) cashe = cashe->next_;
				cashe->next_ = last_item_;
			}
		}
		else {
			last_item_->next_ = new_plus_assert(i, nullptr, last_item_);
			last_item_ = last_item_->next_;
		}
	}
	inline void push_front(Type i) {
		emplace_back(i);
	}
	Type front() {
		assert(pop_possible());
		return first_item_->val_;
	}
	Type pop_front() {
		assert(pop_possible());
		Item* cashe = first_item_;
		first_item_ = first_item_->next_;
		Type out = cashe->val_;
		delete cashe;
		return out;
	}
	Type back() {
		assert(pop_possible());
		return last_item_->val_;
	}
	Type pop_back() {
		assert(pop_possible());
		Item* cashe = last_item_;
		last_item_ = last_item_->prev_;
		Type out = cashe->val_;
		delete cashe;
		return out;
	}
private:
	Item* new_plus_assert(Type& i, Item* next, Item* prev) {
		Item* ptr = new Item(i, next, prev);
		assert(ptr);
		return ptr;
	}
	bool pop_possible() {
		if (first_item_ == nullptr && last_item_ == nullptr) {
			return false;
		}
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
	Item* first_item_;
	Item*  last_item_;
};