#pragma once
#ifndef _STRING_
#include <string>
#endif // ifndef _STRING_
#if !defined _ASSERT_
#define _ASSERT_(expr, msg) static_assert(expr, const_cast<char*> (std::string("ERROR: ") + msg + " | ON LINE: " + __LINE__ + " | IN FILE: " + __FILE__ + '\n').c_str())
#endif // ifndef _ASSERT_