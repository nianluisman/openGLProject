#include "objectLoader.h"
#include <iostream>

void objectLoader::processMesh(aiMesh* mesh, std::vector<float>& verticesModel) {
    // Extract the vertex data from the mesh and add it to the vector
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        if (mesh != nullptr) {
            aiVector3D pos = mesh->mVertices[i];

            verticesModel.push_back(pos.x);
            verticesModel.push_back(pos.y);
            verticesModel.push_back(pos.z);;

        }
    }
    //   verticesModel.push_back(textCoord->x);
//           verticesModel.push_back(textCoord->y);
}

void objectLoader::processNode(aiNode* node, const aiScene* scene, std::vector<float>& _verticesModel) {
    // Process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, _verticesModel);
    }

    // Recursively process all the child nodes
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, _verticesModel);
    }
}


std::vector<float> objectLoader::loadModel(const char* filepath) {
    std::vector<float> verticesVector;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, 0);

    // Check if the file was loaded successfully
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model file: " << importer.GetErrorString() << std::endl;
        return verticesVector; 
    }
    // Extract mesh data

    // Process the model data
    processNode(scene->mRootNode, scene, verticesVector);

    return verticesVector;
}
void objectLoader::setBuffers(const std::vector<float>& vertices,  std::string option ) {
    if (option == "Color") {
        glBindVertexArray(0);
        glGenVertexArrays(1, _VAO);
        glBindVertexArray(*_VAO);
        glGenBuffers(1, &_VBO);

        //// Bind the VBO and pass the vertex data to it
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);

        //// Specify the vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //// Enable the vertex attributes
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else if (option == "Texture") {

        glGenVertexArrays(1, _VAO);
        glBindVertexArray(*_VAO);

        glGenBuffers(1, &_VBO);
        //// Bind the VBO and pass the vertex data to it
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        //// Specify the vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else {
        std::cout << "Buffer option not found" << std::endl;
    }
}
