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
	Deque(T& i) : first_item_(new_plus_assert(i, nullptr, nullptr)), last_item_(nullptr) {}
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
//	Item* new_plus_assert(T& i, Item* next, Item* prev)

	Item* back_ptr() {
		return last_item_;
	}
	void emplace_front(T& i) {
		if (first_item_ == nullptr) {
			first_item_ = new_plus_assert(i, last_item_, nullptr);
			// Link checking
			Item* cashe = last_item_;
			while (cashe != nullptr) {
				cashe = cashe->prev_;
			}
			if (cashe != nullptr) {
				cashe->prev_ = first_item_;
			}
			break;
		}
		first_item_->prev_ = new_plus_assert(i, first_item_, nullptr);
		first_item_ = first_item_->prev_;
	}
	inline void push_back(T i) {
		emplace_back(i);
	}
	void emplace_back(T& i) {
		if (last_item_ == nullptr) {
			last_item_ = new_plus_assert(i, nullptr, first_item_);
			// Link checking
			Item* cashe = first_item_;
			while (cashe != nullptr) {
				cashe = cashe->next_;
			}
			if (cashe != nullptr) {
				cashe->next_ = last_item_;
			}
			break;
		}
		last_item_->next_ = new_plus_assert(i, nullptr, last_item_);
		last_item_ = last_item_->next_;
	}
	inline void push_front(T i) {
		emplace_back(i);
	}
	T front() {
		assert(first_item_);
		return first_item_->val_;
	}
	T pop_front() {
		assert(first_item_);
		Item* cashe = first_item_;
		first_item_ = first_item_->next_;
		T out = cashe->val_;
		delete cashe;
		return out;
	}
	T back() {
		assert(last_item_);
		return last_item_->val_;
	}
	T pop_back() {
		assert(last_item_);
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
