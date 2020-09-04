#include "renderer_backend.h"


static float backdrop_red = 0.45f;
static float backdrop_green = 0.55f;
static float backdrop_blue = 0.60f;

void set_backdrop(float r, float g, float b) {
    backdrop_red = r;
    backdrop_green = g;
    backdrop_blue = b;
}

s_color get_backdrop() {
    s_color bd = {
        .r = backdrop_red,
        .g = backdrop_green,
        .b = backdrop_blue
    };
    return bd;
}