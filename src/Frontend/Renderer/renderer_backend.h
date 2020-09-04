#ifndef GC__RENDERER_BACKEND_H
#define GC__RENDERER_BACKEND_H

typedef struct s_color {
    float r, g, b;
} s_color;

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>
#include <glad/glad.h>

void set_backdrop(float r, float g, float b);
s_color get_backdrop();

#ifdef __cplusplus
}
#endif

#endif //GC__RENDERER_BACKEND_H
