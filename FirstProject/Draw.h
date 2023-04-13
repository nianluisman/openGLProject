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
class Draw
{
public:
	Draw() {};
	void drawCube();
	void drawTower();
	void drawPyramid();
};

