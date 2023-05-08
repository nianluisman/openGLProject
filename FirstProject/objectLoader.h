#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

class objectLoader
{
public:
	objectLoader() {}
	~objectLoader() {}
	void processMesh(aiMesh* mesh, std::vector<float>& verticesModel);
	void processNode(aiNode* node, const aiScene* scene, std::vector<float>& _verticesModel);
	std::vector<float> loadModel(const char* filepath);
};

