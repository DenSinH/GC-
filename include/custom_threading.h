#ifndef GC__CUSTOM_THREADING_H
#define GC__CUSTOM_THREADING_H

#include <stdbool.h>

#ifdef _WIN32

#include "windows.h"

typedef HANDLE mutex;

static mutex create_mutex(bool owned) {
    return CreateMutex(NULL, owned ? TRUE : FALSE, NULL);
}

static bool acquire_mutex(mutex* m) {
    DWORD wait_result = WaitForSingleObject(*m, INFINITE);

    return true;
}

static bool release_mutex(mutex* m) {
    return ReleaseMutex(*m);
}

#else
#error "unimplemented: thread syncing"
#endif

#endif //GC__CUSTOM_THREADING_H
