#ifndef GC__UITHREAD_H
#define GC__UITHREAD_H

#ifdef _WIN32

#include <windows.h>

#define START_DEBUG CreateThread(NULL, 0, UI_thread, NULL, 0, NULL);

DWORD WINAPI UI_thread(LPVOID _);

#else

#include <pthread.h>

#define START_DEBUG pthread_t ui_thread; pthread_create(&ui_thread, NULL, UI_thread, NULL);

void* UI_thread(void* arg);

#endif

#endif //GC__UITHREAD_H
