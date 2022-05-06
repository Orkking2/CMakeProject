#include "Assert.h"

_NSTD _NSTD_AHA<char>& assert_str(const char*& msg, const char*& expr, const int& line, const char*& file) {
	// Standard assert string components
	static const char* err = "ERROR: ", * fr = " | FROM: ", * onl = " | ON LINE: ", * inf = " | IN FILE: ";
	static int err_len = _NSTD c_str_len(err), fr_len = _NSTD c_str_len(fr), onl_len = _NSTD c_str_len(onl), inf_len = _NSTD c_str_len(inf);
	const int num_args = 8; // Must be manually updated

	int msg_len = _NSTD c_str_len(msg), expr_len = _NSTD c_str_len(expr), line_len = 0, file_len = _NSTD c_str_len(file);
	for (int cashe = line; cashe; cashe /= 10)
		line_len++;
	_NSTD _NSTD_AHA<char> line_arr(line_len);

	static const char num_to_str_str[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	for (int i = 0, l = line; i < line_len; i++) {
		line_arr[i] = num_to_str_str[l % 10];
		l /= 10;
	}
	char* c_l = line_arr.get_arr();

	const char* str_args[num_args] = { err, msg, fr, expr, onl, c_l, inf, file };
	const int   args_len[num_args] = { err_len, msg_len, fr_len, expr_len, onl_len, line_len, inf_len, file_len };

	int cashe = 0;
	_NSTD _NSTD_AHA<char> out(err_len + msg_len + fr_len + expr_len + onl_len + line_len + inf_len + file_len);
	_NSTD_FOR(num_args)
		_NSTD copy_to_out(out, str_args[i], args_len[i], cashe);

	return out;
}