#include "DSP.h"

#include "log.h"

#include <memory.h>

#define BASE_FILE_PATH "D:\\Data\\CProjects\\GC-\\src\\Core\\HW\\DSP\\files\\"

int main() {
    s_DSP* DSP = malloc(sizeof(s_DSP));
    memset(DSP, 0, sizeof(s_DSP));

    init_DSP(DSP, BASE_FILE_PATH "dsp_rom.bin", BASE_FILE_PATH "dsp_coef.bin");

    while (1) {
        step_DSP(DSP);
    }

    return 0;
}
