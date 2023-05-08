#include "texureLoader.h"
#include <iostream>
bool texureLoader::loadTexture(const char* filename, unsigned int* textureID) {
    if (!filename || !textureID) {
        return false;
    }
    int width, height, channels;
    unsigned char* image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGB);
    if (!image) {
        std::cout << "Error loading texture " << filename << std::endl;
        return false;
    }
    GLuint texID;
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenerateMipmap(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    *textureID = texID;
    return true;
}

