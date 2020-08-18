#ifndef GC__SYSTEM_H
#define GC__SYSTEM_H

#include "PPC/Gekko.h"

#include "default.h"

typedef struct s_GameCube {
    s_Gekko cpu;
} s_GameCube ;

s_GameCube* init_system();
void run_system(s_GameCube* GameCube);

#endif //GC__SYSTEM_H
