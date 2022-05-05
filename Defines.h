#pragma once
#ifndef _CUSTOM_DEFINES_
#define _CUSTOM_DEFINES_

#define elif(expr) else if (expr)

#ifndef NULL
#define NULL 0
#endif // ifndef NULL

#define _NSTD_FOR(len) for (int i = 0; i < len; i++)

#define _NSTD ::nstd::
#define _NSTD_BEGIN namespace nstd {
#define _NSTD_END }



#endif // ifndef _CUSTOM_DEFINES_