#ifndef GC__FLAGS_H
#define GC__FLAGS_H

#define VERBOSITY_ALL 0
#define VERBOSITY_DEBUG 1
#define VERBOSITY_INFO 2
#define VERBOSITY_WARN 3
#define VERBOSITY_ERROR 4

#define COMPONENT_CPU 0x01
#define COMPONENT_MMU 0x02

// change to change verbosity / component logging:
#define VERBOSITY VERBOSITY_ALL
#define COMPONENT_FLAGS COMPONENT_CPU | COMPONENT_MMU

#define DO_BREAKPOINTS

#endif //GC__FLAGS_H
