#pragma once

#include "Defines.h"
#include <functional>
#include <utility>
#include <vector>
#include <thread>
#include <deque>
#include <mutex>

#ifndef _NSTD_THREADS_
#define _NSTD_THREADS_

_NSTD_BEGIN

template < class R, class... Args >
struct func_wrapper {
	func_wrapper(_STD function <R(Args...)> func, _STD mutex& mut, R* ret = NULL) : func_(func), mut_(mut), ret_(ret) {}
		
	void operator() (Args... args) {
		_STD lock_guard<_STD mutex> ret_guard(mut_);
		if (ret_)
			*ret_ = func_(args);
		else
			func_(args);
	}

	_STD function<R(Args...)> func_;
	_STD mutex mut_;
	R* ret_;
};

class thread_pool {
	typedef _STD pair <_STD function<void(void*)>, void*> _pair;
	/* NOTE:
	* The use of void* as a parameter for each function is done so for abstraction.
	* 
	* Each function must still take void* as a parameter, even if it is not used 
	* within the function.
	* 
	* If the void* parameter is not used, it is recomended to set the associated 
	* pair::second to NULL or nullptr and to simply ignore it within the function.
	* 
	* Functions passed into this thread pool should reinterpret the void*, e.g. 
	* reinterpret_vast<T*> (void* p), before use. 
	* 
	* Object(s) pointed to by the void* pointer is/are NOT protected for 
	* multithreading by default.
	* 
	* Functions passed into the pool should only ever read from the pointer 
	* or use a custom system to protect its/their contents from curroption;
	* see make_thread_safe function below.
	*/

	_STD vector<_STD thread> vThreads_;
	_STD deque<_pair> task_queue_;
	_STD mutex queue_mutex_;
	_STD condition_variable mutex_condition_;
	bool done_;
public:
	template <class... Args>
	struct container {
		_STD size_t size_;
		void** arr_;

		container(const Args*&... args) : size_(sizeof...(args)), arr_(new void*[size_]) {
			reinterpret_cast<void*>(args);
			_NSTD_FOR(size_)
				arr_[i] = 
		}
	};
public:
	thread_pool() : done_(false) {
		vThreads_.resize(
			_STD thread::hardware_concurrency(), 
			_STD thread([this] { thread_loop(); })
		);
	}
	void clear() {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			done_ = true;
		}
		mutex_condition_.notify_all();
		for (_STD thread& thread : vThreads_)
			thread.join();
		vThreads_.clear();
	}
	~thread_pool() {
		clear();
	}
	void thread_loop() {
		_pair task;
		{
			_STD unique_lock<_STD mutex> lock(queue_mutex_);
			mutex_condition_.wait(lock, [this] { return !task_queue_.empty() || done_; });
			if (done_ && task_queue_.empty()) 
				return;
			task = task_queue_.front();
			task_queue_.pop_front();
		}
		task.first(task.second);
	}
	void add_task(const _STD function<void(void*)>& func, void* data = NULL) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			task_queue_.emplace_back(_pair(func, data));
		}
		mutex_condition_.notify_one();
	}
	void add_task(const _pair& pair) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			task_queue_.emplace_back(pair);
		}
		mutex_condition_.notify_one();
	}
	void add_task(_STD vector<_pair> task_list) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			for (_pair task : task_list)
				task_queue_.push_back(task);
		}
		mutex_condition_.notify_all();
	}

	template <class R, class _Ty>
	// Alignment requirements of R must be no stricter than those of _Ty.
	_NODISCARD _STD function<void(void*)> make_thread_safe(const _STD function<R(_Ty*)>& func, _STD mutex& mutex) {
		return _STD function<void(void*)>(
			[&func, &mutex](void* p) { 
				_STD lock_guard<_STD mutex> ret_guard(mutex); 
				*reinterpret_cast<R*>(p) = func(reinterpret_cast<_Ty*> (p))
			}
		);
	}

	template <class R, class... Args>
	_NODISCARD _STD function<void(void*)> make_thread_safe(const _STD function<R(Args...)>& func, _STD mutex& mutex) {

	}
};

_NSTD_END
#endif // !_NSTD_THREADS_