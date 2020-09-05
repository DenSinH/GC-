#include "GX_FIFO.h"
#include "core_utils.h"

HW_REG_WRITE_CALLBACK(write_GX_FIFO, GX_FIFO) {
    GX_FIFO->buffer_size += size >> 3;  // size is in bits

    if (GX_FIFO->buffer_size >= 32) {
        // pass threshold: send commands to the CP
        execute_buffer(GX_FIFO->CP_ptr, GX_FIFO->buffer, GX_FIFO->buffer_size);

        // reset buffer
        GX_FIFO->buffer_size = 0;
    }
}

HW_REG_INIT_FUNCTION(GX_FIFO) {
    GX_FIFO->write = write_GX_FIFO;
}