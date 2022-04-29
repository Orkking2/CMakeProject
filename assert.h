#pragma once
#if !defined _ASSERT_ && defined _STRING_
#define _ASSERT_(expr, msg) static_assert(expr, const_cast<char*> (std::string("ERROR: ") + msg + " | ON LINE: " + __LINE__ + " | IN FILE: " + __FILE__ + '\n').c_str())
#elif !defined _ASSERT_ // ^^^^ defined _STRING_ / !defined _STRING_ vvvv
#ifndef _STRING_TERMINATOR_CHAR_
#define _STRING_TERMINATOR_CHAR_ '\0'
#endif // ifndef _STRING_TERMINATOR_CHAR_
class _Char_manager {
private:
	char* _conts = NULL;
public:
	_Char_manager(char* msg) {
		int len = 0;
		for (char* curr_ptr = msg; *curr_ptr != _STRING_TERMINATOR_CHAR_; curr_ptr++) len++;
		_conts = new char[len + 1];
		for (int i = 0; i < len; i++) _conts[i] = msg[i];
		_conts[len + 1] = _STRING_TERMINATOR_CHAR_;
	}
	~_Char_manager() {
		delete[] _conts;
	}
	char* str() {
		return _conts;
	}
	void operator + (char* string) {
		int str_len = get_len(string);
		int msg_len = get_len(_conts);
		char* cashe_ptr = _conts;
		_conts = new char[str_len + msg_len + 1];
		for (int i = 0; i < msg_len; i++) _conts[i] = cashe_ptr[i]; 
		for (int i = 0; i < str_len; i++) _conts[msg_len + i] = string[i];
		_conts[msg_len + str_len + 1] = _STRING_TERMINATOR_CHAR_;
		delete[] cashe_ptr;
	}
private:
	int get_len(char* str, char terminator = _STRING_TERMINATOR_CHAR_) {
		int len = 0;
		for (char* curr_ptr = str; *curr_ptr != terminator; curr_ptr++) len++;
		return len;
	}
};





#endif // ifndef _ASSERT_