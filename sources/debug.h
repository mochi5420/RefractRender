#ifndef _TSM_DEBUG_H_
#define _TSM_DEBUG_H_

#include <cstdarg>
#include <wchar.h>
#include <Windows.h>

inline void DebugWindow(const char* format, ...) {
	va_list args;
	va_start (args, format);

	char msg[1024];
	wvsprintf(msg, format, args);
	MessageBox(0, msg, "", MB_OK);

	va_end(args);
}

#endif  // _TSM_DEBUG_H_
