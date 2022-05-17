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
	func_wrapper(_STD function <R(Args...)> func, _STD mutex* mut, R* ret = NULL) : func_(func), mut_(mut), ret_(ret) {}
		
	void operator() (Args... args) {
		_STD lock_guard<_STD mutex> guard(*mut_);
		if (ret_)
			*ret_ = func_(args);
		else
			func_(args);
	}

	_STD function<R(Args...)> func_;
	_STD mutex* mut_;
	R* ret_;
};

class thread_pool {
	_STD vector<_STD thread> vThreads_;
	_STD deque<_STD pair <_STD function<void(void*)>, void*>> task_queue_;
	_STD mutex queue_mutex_;
	_STD condition_variable mutex_condition_;
	bool done_;
public:
	thread_pool() : done_(false) {
		vThreads_.resize(
			_STD thread::hardware_concurrency(), 
			_STD thread([this] { this->thread_loop(); })
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
		_STD pair <_STD function<void(void*)>, void*> task;
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
			task_queue_.emplace_back(_STD pair<_STD function<void(void*)>, void*>(func, data));
		}
		mutex_condition_.notify_one();
	}
	void add_task(const _STD pair<_STD function<void(void*)>, void*>& pair) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			task_queue_.emplace_back(pair);
		}
		mutex_condition_.notify_one();
	}
	void add_task(_STD vector<_STD pair <_STD function<void(void*)>, void*>> task_list) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			for (_STD pair <_STD function<void(void*)>, void*> task : task_list)
				task_queue_.push_back(task);
		}
		mutex_condition_.notify_all();
	}
};

_NSTD_END
#endif // !_NSTD_THREADS_