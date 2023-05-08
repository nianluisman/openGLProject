#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
class Draw
{
public:
	Draw() {};
	void drawCube();
	void drawCylinder();
	void drawTower(float height, float slices, float coneHeight, float radius, float coneRadius);
	void drawPyramid();
	void drawSphere(GLfloat radius, GLint slices, GLint stacks);
};

