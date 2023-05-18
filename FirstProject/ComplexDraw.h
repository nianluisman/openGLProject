/* 
This class is used for drawing static complex model that use VAO and combine model from the simple draw class.
*/

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "SimpleDraw.h"


class ComplexDraw
{
public:
    ComplexDraw() {}
    ~ComplexDraw() {}
    void drawTrees(unsigned int VAO, unsigned int shaderprogram, unsigned int texture_id, int size);
    void drawHouse(unsigned int VAO, shaderPrograms shaderprogram, unsigned int imageID_Door, unsigned int imageID_Wall, int size);
    void drawTable(unsigned int VAO, shaderPrograms shaderprogram, int size);
    void drawFLoor(unsigned int VAO, shaderPrograms shaderprogram, unsigned int texture_id, int size);
    void drawSpoon(unsigned int VAO, shaderPrograms shaderprogram, unsigned int text_id, int size);
    void drawCrown(unsigned int VAO, shaderPrograms shaderprogram, unsigned int text_id, int size);
    void drawMonkey(unsigned int VAO, shaderPrograms shaderprogram, unsigned int text_id, int size);
    void drawDonot(unsigned int VAO, shaderPrograms shaderprogram, unsigned int text_id, int size);
};

