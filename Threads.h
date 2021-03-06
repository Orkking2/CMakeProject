#pragma once

#include "Defines.h"
#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <thread>
#include <deque>
#include <mutex>

#ifndef _NSTD_THREADS_
#define _NSTD_THREADS_

_NSTD_BEGIN

//template < class R, class... Args >
//struct func_wrapper {
//	func_wrapper(_STD function <R(Args...)> func, _STD mutex& mut, R* ret = NULL) : func_(func), mut_(mut), ret_(ret) {}
//		
//	void operator() (Args... args) {
//		_STD lock_guard<_STD mutex> ret_guard(mut_);
//		if (ret_)
//			*ret_ = func_(args);
//		else
//			func_(args);
//	}
//
//	_STD function<R(Args...)> func_;
//	_STD mutex mut_;
//	R* ret_;
//};

class thread_pool {
	typedef _STD pair <_STD function<void(void*)>, void*> _Pair_fvp;
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
	_STD deque<_Pair_fvp> task_queue_;
	_STD mutex queue_mutex_;
	_STD condition_variable mutex_condition_;
	bool done_;
public:
	thread_pool(unsigned int count = _STD thread::hardware_concurrency()) : done_(false) {
		unsigned int hc = _STD thread::hardware_concurrency();
		if (count > hc)
			count = hc;
		vThreads_.resize(
			count, 
			_STD thread([this] { thread_loop(); })
		);
	}
	void release() {
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
		release();
	}
	_NODISCARD unsigned int num_threads() {
		return vThreads_.size();
	}
	void thread_loop() {
		while (true) {
			_Pair_fvp task;
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
	}
	void add_task(const _STD function<void(void*)>& func, void* data = NULL) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			task_queue_.emplace_back(_Pair_fvp(func, data));
		}
		mutex_condition_.notify_one();
	}
	void add_task(const _Pair_fvp& pair) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			task_queue_.emplace_back(pair);
		}
		mutex_condition_.notify_one();
	}
	void add_task(_STD vector<_Pair_fvp> task_list) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			for (_Pair_fvp& task : task_list)
				task_queue_.push_back(task);
		}
		_NSTD_FOR(task_list.size())
			mutex_condition_.notify_one();
	}
	template <class R, class... Args>
	void add_task(const _STD function<R(Args...)>& func, _STD mutex& tup_mutex, _STD tuple<R*, Args...>& data) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			task_queue_.push_back(_STD make_pair(make_thread_safe_TUPLE(func, tup_mutex), reinterpret_cast<void*> (&data)));
		}
		mutex_condition_.notify_one();
	}
	template <class R, class... Args>
	void add_task(const _STD function<R(Args...)>& func, R* ret, Args&&... args) {
		_STD mutex dummy_mutex;
		add_task(func, dummy_mutex, _STD make_tuple(ret, args...));
	}

	
	// Function wrappers;

	template <class R, class... Args>
	_NODISCARD _STD function<void(void*)> make_thread_safe_TUPLE(const _STD function<R(Args...)>& func, _STD mutex& tuple_mutex) {
		return _STD function<void(void*)>(
			[&func, &tuple_mutex](void* p) {
				_STD lock_guard<_STD mutex> tuple_guard(tuple_mutex);
				_STD tuple<R*, Args...>* t = reinterpret_cast<_STD tuple<R*, Args...>*>(p);
				*_STD get<R*>(*t) = func(_STD get<Args>(*t) ...);
			}
		);
	}

	template <class... Args>
	_NODISCARD _STD function<void(void*)> make_thread_safe_TUPLE_NORETURN(const _STD function<void(Args...)>& func, _STD mutex& tuple_mutex) {
		return _STD function<void(void*)>(
			[&func, &tuple_mutex](void* p) {
				_STD lock_guard<_STD mutex> tuple_guard(tuple_mutex);
				func(_STD get<Args>(*reinterpret_cast<_STD tuple<Args...>*>(p)) ...);
			}
		);
	}
};

_NSTD_END
#endif // !_NSTD_THREADS_