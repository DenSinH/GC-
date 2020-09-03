#include "GX_FIFO.h"

HW_REG_WRITE_CALLBACK(write_GX_FIFO, GX_FIFO) {

}

HW_REG_INIT_FUNCTION(GX_FIFO) {
    GX_FIFO->write = write_GX_FIFO;
}