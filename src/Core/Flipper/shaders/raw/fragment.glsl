// BEGIN fragmentShaderSource

#version 400 core

in vec4 vertexColor;
flat in uint texturePresent;
flat in uint textureOffset;     // start of texture into texture_data

/*
Idea: pass triangle vertex positions as flat in here (just check index %3 and pass 3 vertices)
*/

out vec4 FragColor;

void main()
{
    if (texturePresent == 0) {
        FragColor = vec4(vertexColor);
    }
    else {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
}

// END fragmentShaderSource