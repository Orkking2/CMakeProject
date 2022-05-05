#pragma once
#ifndef _NSTD_ASSERT
#include "Defines.h"
#include "SmartObjects.h"
_NSTD _S_ARR<char> assert_str(char* msg, char* expr, int line, char* file) {
	// Standard strings
	static const char* err = "ERROR: ", * fr = " | FROM: ", * onl = " | ON LINE: ", * inf = " | IN FILE ";
	static int err_len = _NSTD c_str_len(err), fr_len = _NSTD c_str_len(fr), onl_len = _NSTD c_str_len(onl), inf_len = _NSTD c_str_len(inf);
	const int num_args = 8; // Must be manually updated

	int msg_len = _NSTD c_str_len(msg), expr_len = _NSTD c_str_len(expr), line_len = 0, file_len = _NSTD c_str_len(file);
	for (int cashe = line; cashe; cashe /= 10)
		line_len++;
	_NSTD _S_ARR<char> line_arr(line_len);

	static const char num_to_str_str[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	_NSTD_FOR(line_len) {
		line_arr[i] = num_to_str_str[line % 10];
		line /= 10;
	}
	char* c_l = line_arr.get_arr();

	const char* str_args[num_args] = { err, msg, fr, expr, onl, c_l, inf, file };
	const int   args_len[num_args] = { err_len, msg_len, fr_len, expr_len, onl_len, line_len, inf_len, file_len };

	int cashe = 0;
	_NSTD _S_ARR<char> out(err_len + msg_len + fr_len + expr_len + onl_len + line_len + inf_len + file_len);
	_NSTD_FOR(num_args)
		_NSTD copy_to_out(out, str_args[i], args_len[i], cashe);

	return out;
}

_NSTD_BEGIN // Helper funcs

int c_str_len(const char* c_str) {
	int out = 0;
	for (char* ptr = const_cast<char*> (c_str); ptr != '\0'; ptr++)
		out++;
	return out;
}
template <typename _Ty>
void copy_to_out(_NSTD _S_ARR<_Ty>& s_arr, const _Ty* arr, const int len, int& cashe = 0) {
	for (int i = 0; i < len; i++)
		s_arr[cashe + i] = arr[i];
	cashe += len;
}
int int_list_sum(int* arr, int len) {
	int out = 0;
	_NSTD_FOR(len)
		out += arr[i];
	return out;
}
_NSTD_END
// "ERROR: msg | FROM: #expr | ON LINE: __LINE__ | IN FILE: __FILE__ ";
#define _NSTD_ASSERT(expr, msg) static_assert(!!(expr), assert_str(msg, #expr, __LINE__, __FILE__).get_arr())
#endif // ifndef _NSTD_ASSERT