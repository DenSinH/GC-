#include "custom_threading.h"

#ifdef _WIN32
#include "log.h"

void create_mutex(mutex* m, bool owned) {
    *m = CreateMutex(NULL, owned ? TRUE : FALSE, NULL);

    if (!*m) {
        log_fatal("Failed to create a mutex: %lx", GetLastError());
    }
}

void acquire_mutex(mutex* m) {
    DWORD wait_result;
    do {
        wait_result = WaitForSingleObject(*m, INFINITE);
    } while (wait_result != WAIT_OBJECT_0);
}

void release_mutex(mutex* m) {
    ReleaseMutex(*m);
}

void create_wait_event(wait_event* e, bool signaled) {
    *e = CreateEvent(NULL, TRUE, signaled ? TRUE : FALSE, NULL);

    if (!*e) {
        log_fatal("Failed to create an event: %lx", GetLastError());
    }
}

void wait_for_event(wait_event* e) {
    DWORD wait_result;
    do {
        wait_result = WaitForSingleObject(*e, INFINITE);
        if (wait_result != WAIT_OBJECT_0) {
            log_warn("Waiting for event failed: %lx", wait_result);
        }

    } while (wait_result != WAIT_OBJECT_0);
}

bool is_wait_event_set(wait_event* e) {
    return WaitForSingleObject(*e, 0) != WAIT_TIMEOUT;
}

void set_wait_event(wait_event* e) {
    if (!is_wait_event_set(e)) {
        while (!SetEvent(*e)) {
            log_warn("Error setting event: %lx", GetLastError());
        }
    }
}

void clear_wait_event(wait_event* e) {
    if (is_wait_event_set(e)) {
        while (!ResetEvent(*e)) {
            log_warn("Error resetting event: %lx", GetLastError());
        }
    }
}
#else
#error Not implemented
#endif