#pragma once

#include "Defines.h"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>

#ifndef _NSTD_THREADS_
#define _NSTD_THREADS_


_NSTD_BEGIN

template < class R, class... Args >
struct func_wrapper {
	func_wrapper(std::function <R(Args...)> func, std::mutex* mut, R* ret) : func_(func), mut_(mut), ret_(ret) {}
		
	void operator() (Args... args) {
		std::lock_guard<std::mutex> guard(*mut_);
		*ret_ = func_(args);
	}

	std::function<R(Args...)> func_;
	std::mutex* mut_;
	R* ret_;
};

class thread_manager {
public:
	struct _Tid {
		std::thread* pThread;
		unsigned int id;
	};
private:
	_STD vector<_Tid> threads;
public:
	thread_manager() {

	}
};

_NSTD_END
#endif // !_NSTD_THREADS_