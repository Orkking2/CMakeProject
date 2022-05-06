#pragma once
#ifndef _NSTD_ASSERT
#include "Defines.h"

_NSTD_BEGIN
template <class _Ty>
class _AHA {
private:
	_Ty* _arr;
public:
	_AHA(int len) { 
		_arr = new _Ty[len]; 
	}
	~_AHA() { 
		delete[] _arr;       
		_arr = NULL;
	}

	void resize(const int& size) {
		delete[] _arr;
		_arr = new _Ty[size];
	}

	_Ty*& get_arr() { 
		return _arr;         
	}
	_Ty& operator[] (int i) { 
		return _arr[i];      
	}
};
int c_str_len(const char*& c_str) {
	int out = 0;
	for (char* ptr = const_cast<char*> (c_str); ptr != '\0'; ptr++)
		out++;
	return out;
}
int int_len(const int& in) {
	int out = 0;
	for (int i = in; i; i /= 10)
		out++;
	return out;
}
int int_list_sum(int* arr, int len) {
	int out = 0;
	_NSTD_FOR(len)
		out += arr[i];
	return out;
}
template <typename _Ty>
void copy_to_out(_NSTD _AHA<_Ty>& s_arr, const _Ty* arr, const int len, int& cashe = 0) {
	for (int i = 0; i < len; i++)
		s_arr[cashe + i] = arr[i];
	cashe += len;
}
_NSTD_END

char*& assert_str(_NSTD _AHA<char> out, const char*& msg, const char*& expr, const int& line, const char*& file) {
	// NStandard Assert (NSTDA) string components
	static const char
		* err = "ERROR: ", 
		* fr  = " | FROM: ", 
		* onl = " | ON LINE: ", 
		* inf = " | IN FILE: ";
	// Genning lengths in case NSTDA string components are updated
	static const int 
		err_len = _NSTD c_str_len(err), 
		fr_len  = _NSTD c_str_len(fr), 
		onl_len = _NSTD c_str_len(onl), 
		inf_len = _NSTD c_str_len(inf);

	const int num_args = 8; // Must be manually updated
	// Genning lengths of args
	int msg_len  = _NSTD c_str_len(msg),
		expr_len = _NSTD c_str_len(expr),
		file_len = _NSTD c_str_len(file),
		line_len = _NSTD int_len(line);

	// static_cast<char*> (line); {
	_NSTD _AHA<char> line_arr(line_len);
	
	char num_to_str_str[] = _NSTD_ITSL;

	for (int i = 0, l = line; i < line_len; i++) {
		line_arr[i] = num_to_str_str[l % 10];
		l /= 10;
	}
	char* c_l = line_arr.get_arr();
	// }

	const char* str_args[num_args] = { err, msg, fr, expr, onl, c_l, inf, file };
	const int   args_len[num_args] = { err_len, msg_len, fr_len, expr_len, onl_len, line_len, inf_len, file_len };

	int cashe = 0;
	out.resize(err_len + msg_len + fr_len + expr_len + onl_len + line_len + inf_len + file_len);
	_NSTD_FOR(num_args)
		_NSTD copy_to_out(out, str_args[i], args_len[i], cashe);

	return out.get_arr();
}
// "ERROR: msg | FROM: #expr | ON LINE: __LINE__ | IN FILE: __FILE__ ";
#define _NSTD_ASSERT(expr, msg) {_NSTD _AHA<char> err_str(); static_assert(!!(expr), assert_str(err_str, msg, #expr, __LINE__, __FILE__)) }
#endif // ifndef _NSTD_ASSERT