#ifndef GC__DEBUGTHREAD_H
#define GC__DEBUGTHREAD_H

#ifdef _WIN32

#include <windows.h>

#define START_DEBUG CreateThread(NULL, 0, debug_thread, NULL, 0, NULL);

DWORD WINAPI debug_thread(LPVOID _);

#else
#error Not implemented
#endif

#endif //GC__DEBUGTHREAD_H
