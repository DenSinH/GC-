#ifndef GC__UITHREAD_H
#define GC__UITHREAD_H

#ifdef _WIN32

#include <windows.h>

#define START_DEBUG CreateThread(NULL, 0, UI_thread, NULL, 0, NULL);

DWORD WINAPI UI_thread(LPVOID _);

#else
#error Not implemented
#endif

#endif //GC__UITHREAD_H
