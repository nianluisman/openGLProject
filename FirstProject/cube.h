#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

class cube
{
private:
    unsigned int EBO;
    unsigned int VBO;
public:
    unsigned int VAOcube;

    float vertices[64] = {
        // position              color            texCoord
          -0.5f, -0.5f, -0.5f     ,1.0f, 0.0f, 0.0f    ,0.0f, 0.0f, //vertex 0 
          0.5f, -0.5f, -0.5f      ,0.0f, 1.0f, 0.0f    ,1.0f, 0.0f,
          0.5f,  0.5f, -0.5f      ,0.0f, 0.0f, 1.0f    ,1.0f, 1.0f,
          -0.5f,  0.5f, -0.5f     ,1.0f, 1.0f, 0.0f    ,0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f     ,0.0f, 1.0f, 1.0f    ,1.0f, 0.0f,
          0.5f, -0.5f,  0.5f      ,1.0f, 0.0f, 1.0f    ,0.0f, 0.0f,
          0.5f,  0.5f,  0.5f      ,0.5f, 0.5f, 0.5f    ,0.0f, 1.0f,
          -0.5f,  0.5f,  0.5f     ,1.0f, 1.0f, 1.0f    ,1.0f, 1.0f
    };

    unsigned int indices[36] = {
        0, 1, 2, // front
        2, 3, 0,
        1, 5, 6, // right
        6, 2, 1,
        7, 6, 5, // back
        5, 4, 7,
        4, 0, 3, // left  
        3, 7, 4,
        4, 5, 1, // bottom
        1, 0, 4,
        3, 2, 6, // top
        6, 7, 3
    };

    void setBuffer();
};

