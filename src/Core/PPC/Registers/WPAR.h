#ifndef GC__WPAR_H
#define GC__WPAR_H

#define WPAR_WRITE_MASK 0xffffffe0

typedef union s_WPAR {
    // integer:
    struct {
        unsigned BNE: 1;
        unsigned: 4;        // reserved
        unsigned GB_ADDR: 27;
    };
    unsigned raw;
} s_WPAR;

#endif //GC__WPAR_H
