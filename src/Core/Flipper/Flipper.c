#include "Flipper.h"

#include "shaders/shaders.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <glad.h>


void init_Flipper(s_Flipper* flipper) {
    flipper->backdrop.r = 0.45;
    flipper->backdrop.g = 0.55f;
    flipper->backdrop.b = 0.60f;
}

void video_init_Flipper(s_Flipper* flipper) {
// set up openGL stuff
    // create shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // compile it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // debugging
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed: %s\n", infoLog);
    }

    // create and compile fragmentshader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // debugging
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader compilation failed: %s\n", infoLog);
    }

    // create program object
    flipper->shaderProgram = glCreateProgram();
    glAttachShader(flipper->shaderProgram, vertexShader);
    glAttachShader(flipper->shaderProgram, fragmentShader);
    glLinkProgram(flipper->shaderProgram);

    // debugging
    glGetProgramiv(flipper->shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(flipper->shaderProgram, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
    }

    // now that we have linked the shaders we dont need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] =
    {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f,

            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.0f, -0.5f, 0.0f
    };

    // Setup OpenGL buffers
    glGenBuffers(1, &flipper->VBO); // create buffer

    // Setup OpenGL vertex array object
    glGenVertexArrays(1, &flipper->VAO);
    glBindVertexArray(flipper->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, flipper->VBO);   // bind to array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // buffer data

    // set vertex attribute pointers to the right format
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(flipper->VBO, 0);
}

void render_Flipper(s_Flipper* flipper) {

    glClearColor(flipper->backdrop.r, flipper->backdrop.g, flipper->backdrop.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(flipper->shaderProgram);
    glBindVertexArray(flipper->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

//        unsigned error = glGetError();
//        if (error) {
//            printf("Error: %08x\n", error);
//        }
}