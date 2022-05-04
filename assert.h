#pragma once
#ifndef _NSTD_ASSERT
char* assert_str(char* msg, char* expr, int line, char* file) {
	int len = 0;
	static const int str_arg_num = 3;
	char* ptrptr[str_arg_num] = { msg, expr, file };
	for (int i = 0; i < str_arg_num; i++)
		for (char* ptr = ptrptr[i]; ptr != '\0'; ptr++) 
			len++;
	for (bool done = false; !done;) {

	}
}
#define _NSTD_ASSERT(expr, msg) static_assert(expr, assert_str(msg, #expr, __LINE__, __FILE__))
#endif // ifndef _NSTD_ASSERT