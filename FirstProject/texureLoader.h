#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2/SOIL2.h>

class texureLoader
{

public:
	texureLoader() {}
	~texureLoader() {}
	bool loadTexture(const char* filename, unsigned int* textureID);
};

