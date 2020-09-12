// BEGIN transformationShaderSource

#version 430 core

layout (std430, binding = 4) buffer XF_SSBO
{
    float XF_A[0x100];
    float XF_B[0x100];
    float XF_C[0x100];
    float XF_D[0x100];
    float XF_regs[0x58];  // some regs are not floats, but most are
};

vec4 transform(vec3 position, uint posidx) {
    // todo: general position matrix
    // this seems to be what libOGC makes from projection matrices
    mat4 projection = mat4(
        XF_regs[%XF_reg_projection_A%], 0, 0, 0,  // first column
        0, XF_regs[%XF_reg_projection_C%], 0, 0,  // second column
        0, 0, XF_regs[%XF_reg_projection_E%], -1,  // third column
        XF_regs[%XF_reg_projection_B%], XF_regs[%XF_reg_projection_D%], XF_regs[%XF_reg_projection_F%], 0  // fourth column
    );

    // matrices can be accessed per row
    uint posmtx_base = posidx << 2;
    mat4 modelview = mat4(
        XF_A[posidx + 0], XF_A[posidx + 4], XF_A[posidx + 8], 0,
        XF_A[posidx + 1], XF_A[posidx + 5], XF_A[posidx + 9], 0,
        XF_A[posidx + 2], XF_A[posidx + 6], XF_A[posidx + 10], 0,
        XF_A[posidx + 3], XF_A[posidx + 7], XF_A[posidx + 11], 1
    );

    vec4 pos = vec4(position, 1);
    pos = projection * modelview * pos;
    pos /= pos.w;

    return pos;
}

// END transformationShaderSource