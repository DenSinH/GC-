// BEGIN transformationShaderSource

#version 430 core

layout (std430, binding = 4) readonly buffer XF_SSBO
{
    float XF_A[0x100];
    float XF_B[0x100];
    float XF_C[0x100];
    float XF_D[0x100];
    float XF_regs[0x58];  // some regs are not floats, but most are
};

vec4 transform_pos(vec3 position, uint posidx) {
    // this seems to be what libOGC makes from projection matrices
    mat4 projection = mat4(
        XF_regs[++XF_reg_projection_A++], 0, 0, 0,  // first column
        0, XF_regs[++XF_reg_projection_C++], 0, 0,  // second column
        0, 0, XF_regs[++XF_reg_projection_E++], 1,  // third column
        XF_regs[++XF_reg_projection_B++], XF_regs[++XF_reg_projection_D++], XF_regs[++XF_reg_projection_F++], 0  // fourth column
    );

    // matrices can be accessed per row
    uint posmtx_base = posidx << 2;
    mat4 modelview = mat4(
        XF_A[posmtx_base + 0], XF_A[posmtx_base + 4], XF_A[posmtx_base + 8], 0,
        XF_A[posmtx_base + 1], XF_A[posmtx_base + 5], XF_A[posmtx_base + 9], 0,
        XF_A[posmtx_base + 2], XF_A[posmtx_base + 6], XF_A[posmtx_base + 10], 0,
        XF_A[posmtx_base + 3], XF_A[posmtx_base + 7], XF_A[posmtx_base + 11], 1
    );

    vec4 pos = vec4(position, 1);
    pos = projection * modelview * pos;
    pos /= pos.w;

    return pos;
}

vec3 transform_tex(vec3 texcoord, uint texidx) {
    // matrices can be accessed per row
    uint texmtx_base = texidx << 2;
    mat3 xf_mat = mat3(
        XF_A[texmtx_base + 0], XF_A[texmtx_base + 4], XF_A[texmtx_base + 8],
        XF_A[texmtx_base + 1], XF_A[texmtx_base + 5], XF_A[texmtx_base + 9],
        XF_A[texmtx_base + 2], XF_A[texmtx_base + 6], XF_A[texmtx_base + 10]
    );

    vec3 coord = vec3(texcoord.xy, 1);
    coord = xf_mat * coord;
    coord /= coord.z;

    return coord;
}

// END transformationShaderSource