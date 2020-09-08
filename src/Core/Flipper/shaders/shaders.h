#ifndef GC__SHADER_H
#define GC__SHADER_H

// fragmentShaderSource (from fragment.vert, lines 0 to 11)
const char* fragmentShaderSource = 
"#version 400 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(vertexColor);\n"
"}\n";


// vertexShaderSource (from vertex.vert, lines 0 to 38)
const char* vertexShaderSource = 
"#version 400 core\n"
"uniform uint VCD;\n"
"uniform uint VAT_A;\n"
"uniform uint VAT_B;\n"
"uniform uint VAT_C;\n"
"\n"
"// layout corresponds to that in GX_constants.h\n"
"layout (location = 0)  in uvec2 pos_2d_u8;\n"
"layout (location = 1)  in ivec2 pos_2d_s8;\n"
"layout (location = 2) in uvec2 pos_2d_u16;\n"
"layout (location = 3) in ivec2 pos_2d_s16;\n"
"layout (location = 4) in vec2  pos_2d_f32;\n"
"\n"
"layout (location = 5)  in uvec3 pos_3d_u8;\n"
"layout (location = 6)  in ivec3 pos_3d_s8;\n"
"layout (location = 7) in uvec3 pos_3d_u16;\n"
"layout (location = 8) in ivec3 pos_3d_s16;\n"
"layout (location = 9) in vec3  pos_3d_f32;\n"
"\n"
"if (true) {\n"
"    // layout (location = 10)   in uvec1 clr0_rgb565;\n"
"    layout (location = 11)   in uvec3 clr0_rgb888;\n"
"    layout (location = 12)  in uvec4 clr0_rgb888x;\n"
"    layout (location = 13) in uvec2 clr0_rgba4444;\n"
"    // layout (location = 14) in uvec1 clr0_rgba6666;\n"
"    layout (location = 15) in uvec4 clr0_rgba8888;\n"
"}\n"
"\n"
"out vec4 vertexColor;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(pos_3d_s16.zyx / 2147483647.0, 1.0);\n"
"    vertexColor = vec4(clr0_rgba8888.wzyx / 2147483647.0);\n"
"}\n";

#endif  // GC__SHADER_H