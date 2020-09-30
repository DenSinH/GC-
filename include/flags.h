#ifndef GC__FLAGS_H
#define GC__FLAGS_H

#define VERBOSITY_ALL 0
#define VERBOSITY_DEBUG 1
#define VERBOSITY_INFO 2
#define VERBOSITY_WARN 3
#define VERBOSITY_ERROR 4
#define VERBOSITY_MAX VERBOSITY_ERROR

#define COMPONENT_CPU_VERBOSE 0x01
#define COMPONENT_MMU         0x02
#define COMPONENT_HW_REGS     0x04
#define COMPONENT_CP          0x08
#define COMPONENT_FLIPPER     0x10
#define COMPONENT_SCHEDULER   0x20
#define COMPONENT_CPU         0x40
#define COMPONENT_SI          0x80

#define SCREEN_NTSC 0
#define SCREEN_PAL 1

#define SCREEN_TYPE SCREEN_NTSC

#ifndef NDEBUG
// change to change verbosity / component logging:
#define VERBOSITY VERBOSITY_WARN
#define COMPONENT_FLAGS (0)

#define CHECK_SPR_ACCESS
#define CHECK_CP_COMMAND
#define CHECK_CP_DATA_BUFFER
#undef RECURSE_HR_ACCESS
#define DO_ASSERT_PAIRED_SINGLE

#define DO_DEBUGGER
#define DO_BREAKPOINTS
#define DO_CALL_STACK
#else
#define VERBOSITY VERBOSITY_MAX
#endif

#endif //GC__FLAGS_H
