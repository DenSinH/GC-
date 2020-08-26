#include "debugthread.h"
#include "src/Debugger/debugger.h"

#ifdef _WIN32

DWORD WINAPI debug_thread(LPVOID _) {
    debugger_run();
    return 0;
}

#else
#error Not implemented
#endif