#pragma once

#include "Defines.h"
#include "assert.h"
#include <deque>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>

#ifndef _NSTD_THREADS_
#define _NSTD_THREADS_

#define _NSTD_TERMINATOR_THREAD_ID_ 0U

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

class thread_manager {
	_STD vector<_STD thread> vThreads_;
	_STD deque<_STD function<void(void*)>> task_queue_;
	_STD mutex queue_mutex_;
	_STD condition_variable mutex_condition_;
	void* pData_;
public:
	thread_manager() {
		vThreads_.resize(std::thread::hardware_concurrency());
		for (std::thread& t : vThreads_)
			t = std::thread(thread_loop, pData_);
	}
	void thread_loop(void*) {

	}
};

/*
class thread_manager {
	_STD vector<unsigned int> t_ids_;
	_STD unordered_map<unsigned int, _STD thread*> map_;
	_STD mutex mutex_;
public:
	inline void terminate_thread(_STD thread* pThread) {
		(*pThread).join();
	}
	inline void terminate_threads(_STD vector<_STD thread*> vThreads) {
		for (_STD thread* pThread : vThreads)
			terminate_thread(pThread);
	}
public:
	thread_manager() {

	}

	bool is_id(unsigned int id) {
		for (unsigned int ids : t_ids_)
			if (ids == id)
				return true;
		return false;
	}
	_NODISCARD bool add_thread(unsigned int id, _STD thread* pThread) {
		is_id(id) ? return false; : ;
		t_ids_.push_back(id);
		map_[id] = pThread;
		return true;
	}
	_NODISCARD unsigned int add_thread(_STD thread* pThread) {

	}
	unsigned int gen_id() {
		for (unsigned int cashe = 1U; true; cashe++)
			if (!is_id(cashe))
				return cashe;
		return 0U;
	}
	bool join(unsigned int id) {
		is_id(id) ? ; : return false;
		(*map_[id]).join();
		t_ids_.erase(find_id(id));
		return true;
	}
	bool join(_STD vector<unsigned int> ids) {
		bool flag = true;
		for (unsigned int id : ids)
			if (!join(id))
				flag = false;
		return flag;
	}
	bool no_block_join(_STD vector<unsigned int> ids) {
		(*map_[_NSTD_TERMINATOR_THREAD_ID_]).join();
		bool flag = true;
		_STD vector<_STD thread*> vThreads = {};
		for (unsigned int id : ids)
			if (is_id(id))
				vThreads.emplace_back(map_[id]);
			else
				flag = false;
		std::thread thread(terminate_threads(vThreads));
		map_[_NSTD_TERMINATOR_THREAD_ID_] = &thread;
		return flag;
	}
	~thread_manager() {
		for (unsigned int ids : t_ids_)
			(*map_[ids]).join();
	}
private:
	_STD vector<unsigned int>::iterator find_id(unsigned int id) {
		for (_STD vector<unsigned int>::iterator it = t_ids_.begin(); ; it++)
			if (t_ids_[it] == id)
				return it;
	}
};
*/

_NSTD_END
#endif // !_NSTD_THREADS_