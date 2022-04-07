#pragma once

template <typename T>
struct Item {
	Item(T& val, Item* next = nullptr, Item* prev = nullptr) : val_(val), next_(next), prev_(prev) {}
	T val_;
	Item* next_;
	Item* prev_;
};

template <typename T>
class Deque {
public:
	using Item = Item<T>;
	Deque() : first_item_(nullptr), last_item_(nullptr) {}
	Deque(T& i) : first_item_(new_plus_assert(i, nullptr, nullptr)), last_item_(first_item_) {}
	Deque(Deque& d) : first_item_(d.front_ptr()), last_item_(d.back_ptr()) {}
	~Deque() {
		while (first_item_) {
			Item* cashe = first_item_;
			first_item_ = first_item_->next_;
			delete cashe;
		}
	}
	Item* front_ptr() {
		return first_item_;
	}
	Item* back_ptr() {
		return last_item_;
	}
	void emplace_front(T& i) {
		first_item_->prev_ = new_plus_assert(i, first_item_, nullptr);
		first_item_ = first_item_->prev_;
	}
	inline void push_back(T i) {
		emplace_back(i);
	}
	void emplace_back(T& i) {
		last_item_->next_ = new_plus_assert(i, nullptr, last_item_);
		last_item_ = last_item_->next_;
	}
	inline void push_front(T i) {
		emplace_back(i);
	}
	T front() {
		return first_item_->val_;
	}
	T pop_front() {
		Item* cashe = first_item_;
		first_item_ = first_item_->next_;
		T out = cashe->val_;
		delete cashe;
		return out;
	}
	T back() {
		return last_item_->val_;
	}
	T pop_back() {
		Item* cashe = last_item_;
		last_item_ = last_item_->prev_;
		T out = cashe->val_;
		delete cashe;
		return out;
	}
private:
	Item* new_plus_assert(T& i, Item* next, Item* prev) {
		Item* ptr = new Item(i, next, prev);
		assert(ptr);
		return ptr;
	}
	Item* first_item_;
	Item*  last_item_;
};