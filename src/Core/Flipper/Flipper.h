#ifndef GC__FLIPPER_H
#define GC__FLIPPER_H

typedef struct s_Flipper {

    struct {
        float r, g, b;
    } backdrop;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int shaderProgram;

} s_Flipper;

void init_Flipper(s_Flipper* flipper);

void video_init_Flipper(s_Flipper* gpu);
void render_Flipper(s_Flipper* flipper);

#endif //GC__FLIPPER_H
