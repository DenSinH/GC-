// BEGIN fragmentShaderSource

#version 330 core
out vec4 FragColor;
in vec4 vertexColor;

void main()
{
    FragColor = vec4(vertexColor.xy, 0.0, 0.0);
}

// END fragmentShaderSource