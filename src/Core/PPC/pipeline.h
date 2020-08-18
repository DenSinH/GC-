#ifndef GC__PIPELINE_H
#define GC__PIPELINE_H

#include "default.h"

typedef union s_pipeline {
    struct {
        u8 head : 2;
        u32 buffer[4];
    };
} s_pipeline;

#define get_dispatch(pipeline) pipeline.buffer[(pipeline.head - 2) & 3];
#define set_prefetch(pipeline, prefetch) pipeline.buffer[pipeline.head++] = prefetch;

#endif //GC__PIPELINE_H
