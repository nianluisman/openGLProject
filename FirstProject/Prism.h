#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Prism
{
private:
    unsigned int VBOPrism, EBOPrism;

public:
    unsigned VAOprism;

    float vertices[144] = {
        // Front triangle
           -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   // Vertex 0 (Position and Color)
            0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   // Vertex 1 (Position and Color)
            0.0f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   // Vertex 2 (Position and Color)

            // Back triangle
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // Vertex 3 (Position and Color)
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   // Vertex 4 (Position and Color)
             0.0f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   // Vertex 5 (Position and Color)

             // Connecting edges
             -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   // Vertex 6 (Position and Color)
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   // Vertex 7 (Position and Color)

              0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   // Vertex 8 (Position and Color)
              0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   // Vertex 9 (Position and Color)

              0.0f,  0.5f, 0.5f,   0.5f, 0.5f, 0.0f,   // Vertex 10 (Position and Color)
              0.0f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f    // Vertex 11 (Position and Color)
    };

    unsigned int  indices[42] = {
        // Front face
    0, 1, 2,

     // Back face
     3, 4, 5,

     // Side faces
    0, 1, 3,
    1, 4, 3,
    1, 2, 4,
    2, 5, 4,
    2, 0, 5,
    0, 3, 5
    };

    void setBuffer();
};

