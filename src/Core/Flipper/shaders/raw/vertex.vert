// BEGIN vertexShaderSource

#version 400 core
uniform uint VCD;
uniform uint VAT_A;
uniform uint VAT_B;
uniform uint VAT_C;

// layout corresponds to that in GX_constants.h
layout (location = %POS_2D_U8%)  in uvec2 pos_2d_u8;
layout (location = %POS_2D_S8%)  in ivec2 pos_2d_s8;
layout (location = %POS_2D_U16%) in uvec2 pos_2d_u16;
layout (location = %POS_2D_S16%) in ivec2 pos_2d_s16;
layout (location = %POS_2D_F32%) in vec2  pos_2d_f32;

layout (location = %POS_3D_U8%)  in uvec3 pos_3d_u8;
layout (location = %POS_3D_S8%)  in ivec3 pos_3d_s8;
layout (location = %POS_3D_U16%) in uvec3 pos_3d_u16;
layout (location = %POS_3D_S16%) in ivec3 pos_3d_s16;
layout (location = %POS_3D_F32%) in vec3  pos_3d_f32;

// layout (location = %CLR0_rgb565%)   in uvec1 clr0_rgb565;
layout (location = %CLR0_rgb888%)   in uvec3 clr0_rgb888;
layout (location = %CLR0_rgb888x%)  in uvec4 clr0_rgb888x;
layout (location = %CLR0_rgba4444%) in uvec2 clr0_rgba4444;
// layout (location = %CLR0_rgba6666%) in uvec1 clr0_rgba6666;
layout (location = %CLR0_rgba8888%) in uvec4 clr0_rgba8888;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(pos_3d_s16.zyx / 2147483647.0, 1.0);
    vertexColor = vec4(clr0_rgba8888.wzyx / 2147483647.0);
}

// END vertexShaderSource