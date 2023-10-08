#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H

#if _WIN32
#include <direct.h>
#include <Windows.h>
#define sb_get_work_directory _getcwd
#define sb_sleep Sleep
#define SB_FIX_UNICODE() SetConsoleOutputCP(CP_UTF8)
#else
#include <unistd.h>
#define sb_get_work_directory getcwd
#define sb_sleep sleep
#define SB_FIX_UNICODE()
#endif

#if __GNUC__ >= 4
#define SB_EXTERN(type) extern \
			 __attribute__((visibility("default"))) \
			 type
#elif defined(_MSC_VER)
#define SB_EXTERN(type) __declspec(dllexport) type __cdecl
#else
#define SB_EXTERN(type) extern type
#endif

typedef enum {
	SB_OK = (0),
	SB_ERROR = (-1),
} sb_code;
#define SB_DEF_EXTERN SB_EXTERN(sb_code)

#endif
