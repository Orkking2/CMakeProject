#pragma once
#ifndef _ASSERT_
char* assert_str(char* msg, char* expr, int line, char* file) {
	
}
#define _ASSERT_(expr, msg) static_assert(expr, assert_str(msg, #expr, __LINE__, __FILE__))
#endif // ifndef _ASSERT_