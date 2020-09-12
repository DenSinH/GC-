// BEGIN fragmentShaderSource

#version 400 core
out vec4 FragColor;
in vec4 vertexColor;

void main()
{
    FragColor = vec4(vertexColor);
}

// END fragmentShaderSource