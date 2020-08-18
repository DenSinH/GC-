#include <stdio.h>

#include "src/IPL/loader.h"

int main() {
    u8* IPL = decrypt_IPL("./files/ipl.bin");
    dump_IPL(IPL, "./files/ipl.dump");
    free_IPL(IPL);
    return 0;
}
