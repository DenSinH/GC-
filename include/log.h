#ifndef GC__LOG_H
#define GC__LOG_H

#include <stdio.h>
#include <stdlib.h>

#define VERBOSITY_ALL 0
#define VERBOSITY_DEBUG 1
#define VERBOSITY_INFO 2
#define VERBOSITY_WARN 3
#define VERBOSITY_ERROR 4

// change to change verbosity:
#define VERBOSITY VERBOSITY_ALL


#ifdef _WIN32
    #include <windows.h>

    #define FOREGROUND_YELLOW (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN)
    #define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

    #define CONSOLE_BLUE() { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY); }
    #define CONSOLE_YELLOW() { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_YELLOW); }
    #define CONSOLE_RED() { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); }
    #define CONSOLE_RESTORE() { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_WHITE); }

#else

    // linux / macOS
    #define CONSOLE_YELLOW() { fprintf(stdout, "\033[0;36m"); }
    #define CONSOLE_YELLOW() { fprintf(stdout, "\033[0;33m"); }
    #define CONSOLE_RED() { fprintf(stderr, "\033[1;31m"); }
    #define CONSOLE_RESTORE() { fprintf(stdout, "\033[0m"); }

#endif

#if VERBOSITY <= VERBOSITY_ALL
    #define log(message, ...) {                        \
        CONSOLE_BLUE();                                \
        fprintf(stdout, message "\n",  ##__VA_ARGS__); \
        CONSOLE_RESTORE();                             \
    }
#else
    #define log(message, ...) { }
#endif

#if VERBOSITY <= VERBOSITY_DEBUG
    #define log_debug(message, ...) {                 \
        fprintf(stdout, "[DEBUG]:");                  \
        fprintf(stdout, message "\n", ##__VA_ARGS__); \
    }
#else
    #define log_debug(message, ...) {}
#endif

#if VERBOSITY <= VERBOSITY_INFO
#define log_info(message, ...) {                 \
        fprintf(stdout, "[INFO]:");                  \
        fprintf(stdout, message "\n", ##__VA_ARGS__); \
    }
#else
#define log_info(message, ...) {}
#endif

#if VERBOSITY <= VERBOSITY_WARN
    #define log_warn(message, ...) {                  \
        CONSOLE_YELLOW();                             \
        fprintf(stdout, "[WARN]:");                   \
        fprintf(stdout, message "\n", ##__VA_ARGS__); \
        CONSOLE_RESTORE();                            \
    }
#else
    #define log_warn(message, ...) { }
#endif


#if VERBOSITY <= VERBOSITY_ERROR
    #define log_fatal(message, ...) {                                \
        CONSOLE_RED();                                               \
        fprintf(stderr, "[FATAL] at %s:%d: ", __FILE__, __LINE__);   \
        fprintf(stderr, message "\n", ##__VA_ARGS__);                \
        CONSOLE_RESTORE();                                           \
        exit(1);                                                     \
    }
#else
    #define log_fatal(message, ...) { }
#endif

#endif //GC__LOG_H
