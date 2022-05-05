#pragma once
#ifndef _NSTD_ASSERT
#include "Defines.h"
#include "SmartObjects.h"
_NSTD _S_ARR<char> assert_str(char* msg, char* expr, int line, char* file) {
	// Standard strings
	const char* err = "ERROR: ", * fr = " | FROM: ", * onl = " | ON LINE: ", * inf = " | IN FILE ";
	const int err_len = 7, fr_len = 9, onl_len = 12, inf_len = 12;

	int msg_len = _NSTD c_str_len(msg), expr_len = _NSTD c_str_len(expr), line_len = 0, file_len = _NSTD c_str_len(file);
	for (int cashe = line; cashe; cashe /= 10)
		line_len++;
	_NSTD _S_ARR<char> out(err_len + msg_len + fr_len + expr_len + onl_len + line_len + inf_len + file_len);
	_NSTD _S_ARR<char> line_arr(line_len);

	const char num_to_str_str[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	for(int i = 0, )

	char* c_l = line_arr.get_arr();

	int cashe = 0;
	_NSTD copy_to_out(out,  err,  err_len, cashe);
	_NSTD copy_to_out(out,  msg,  msg_len, cashe);
	_NSTD copy_to_out(out,   fr,   fr_len, cashe);
	_NSTD copy_to_out(out, expr, expr_len, cashe);
	_NSTD copy_to_out(out,  onl,  onl_len, cashe);
	_NSTD copy_to_out(out,  c_l, line_len, cashe);
	_NSTD copy_to_out(out,  inf,  inf_len, cashe);
	_NSTD copy_to_out(out, file, file_len, cashe);


	return out;
}
_NSTD_BEGIN
int pow(int base, int power) {
	if (power <= 0)
		return 1;
	int out = base;
	for (int i = 0; i < power; i++)
		out *= base;
	return out;
}
int c_str_len(char* c_str) {
	int out = 0;
	for (char* ptr = c_str; ptr != '\0'; ptr++)
		out++;
	return out;
}
template <typename _Ty>
void copy_to_out(_NSTD _S_ARR<_Ty>& s_arr, const _Ty* arr, const int len, int& cashe = 0) {
	for (int i = 0; i < len; i++)
		s_arr[cashe + i] = arr[i];
	cashe += len;
}
_NSTD_END
// "ERROR: " + msg + " | FROM: " + #expr + " | ON LINE: " + __LINE__ + " | IN FILE: " + __FILE__ ;
#define _NSTD_ASSERT(expr, msg) static_assert(!!(expr), assert_str(msg, #expr, __LINE__, __FILE__).get_arr())
#endif // ifndef _NSTD_ASSERT