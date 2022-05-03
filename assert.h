#pragma once
#if !defined _ASSERT_ && defined _STRING_
#define _ASSERT_(expr, msg) static_assert(expr, const_cast<char*> (std::string("ERROR: ") + msg + " | ON LINE: " + __LINE__ + " | IN FILE: " + __FILE__ + '\n').c_str())
#elif !defined _ASSERT_ // ^^^^ defined _STRING_ / !defined _STRING_ vvvv
#ifndef _STRING_TERMINATOR_CHAR_
#define _STRING_TERMINATOR_CHAR_ '\0'
#endif // ifndef _STRING_TERMINATOR_CHAR_
#ifndef NULL
#define NULL 0
#endif // ifndef NULL
#define _ASSERT_(expr, msg) static_assert(expr, assert_str_constructor("ERROR: ", msg, " | ON LINE: ", __LINE__, " | IN FILE: ", __FILE__, '\n'))

char* assert_str_constructor(char* err, char* msg, int line, char* in_file, char* file, char new_line) {
	int err_len = get_len(err);
	int msg_len = get_len(msg);
	int inf_len = get_len(in_file);
	int fil_len = get_len(file);
	



}
int get_len(char* str) {
	int out = 0;
	for (char* curr_ptr = str; *curr_ptr != _STRING_TERMINATOR_CHAR_; curr_ptr++) out++;
	return out;
}

#endif // ifndef _ASSERT_