#pragma once

#include "Defines.h"
#include <functional>
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

struct safe_thread {
	std::thread thread_;
	safe_thread(std::thread&  thread) {
		thread_ = thread;
	}
	safe_thread(std::thread&& thread) : thread_(thread) {}

	_NODISCARD static safe_thread make_thread_safe(std::thread&& thread) {
		return safe_thread(thread);
	}

	_NODISCARD static safe_thread make_thread_safe(std::thread& thread) {
		return safe_thread(thread);
	}

	_NODISCARD static safe_thread make_thread_safe(std::thread* pThread) {
		return safe_thread(*pThread);
	}

	~safe_thread() {
		thread_.join();
	}
};

_NSTD_END
#endif // !_NSTD_THREADS_