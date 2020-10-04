// BEGIN framebufferVertexShaderSource

#version 430

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 vtx_texCoord;

out vec2 texCoord;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);

    // flip vertically
    texCoord = vec2(vtx_texCoord.x, 1.0 - vtx_texCoord.y);
}

// END framebufferVertexShaderSource