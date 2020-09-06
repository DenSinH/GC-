#ifndef GC__SHADER_H
#define GC__SHADER_H

// fragmentShaderSource (from fragment.vert, lines 0 to 10)
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


// vertexShaderSource (from vertex.vert, lines 0 to 12)
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"out vec4 vertexColor;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\n";

#endif  // GC__SHADER_H