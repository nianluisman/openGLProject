#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "vertexBuffer.h"

class objectLoader
{
private:
	unsigned int* _VAO;
	unsigned int  _VBO;
public:
	objectLoader(unsigned int* VAO, unsigned int VBO) : _VAO(VAO), _VBO(_VBO) {}
	~objectLoader() {}
	void processMesh(aiMesh* mesh, std::vector<float>& verticesModel);
	void processNode(aiNode* node, const aiScene* scene, std::vector<float>& _verticesModel);
	std::vector<float> loadModel(const char* filepath);
	void setBuffers(const std::vector<float> &vetices, std::string option);
	void render();
};

