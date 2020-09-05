#include "UIthread.h"
#include "src/Frontend/interface.h"

#ifdef _WIN32

DWORD WINAPI UI_thread(LPVOID _) {
    ui_run();
    return 0;
}

#else
#error Not implemented
#endif