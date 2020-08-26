#ifndef GC__THREADING_H
#define GC__THREADING_H

#ifdef _WIN32

#include <windows.h>

#define START_THREAD(target) CreateThread(NULL, 0, target, NULL, 0, NULL)

#else
#error Not implemented
#endif

#endif //GC__THREADING_H
