#pragma once
#ifndef _ASSERT_
#define _ASSERT_

#include <string>

#define _ASSERT(expr, msg) static_assert(expr, const_cast<char*> (std::string("ERROR: ") + msg + " | ON LINE: " + __LINE__ + " | IN FILE: " + __FILE__ + '\n').c_str())






#endif // ifndef _ASSERT_