#include "DSP.h"

#include "log.h"

int main() {
    s_ac ac;
    ac.lmh = 0xFFBBBBAAAA;

    log_dsp("%x", ac.l);
    log_dsp("%x", ac.m);
    log_dsp("%x", ac.h);

    return 0;
}
