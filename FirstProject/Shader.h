#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

struct shaderPrograms {
    unsigned int textureProgram;
    unsigned int ColorProgram;
};

struct ShaderProgramSourse {
    /*Programs read form the basic.shader file*/
    std::string VertexProgram;
    std::string TexureFracmentProgram;
    std::string FracmentColorProgram;
};

class Shader
{

public:
    Shader(){}
    ~Shader(){}
    static ShaderProgramSourse parseShaders(const std::string& filePath);
    static unsigned int CompileShader(unsigned int type, const std::string& sourse);

    shaderPrograms creatShader(const ShaderProgramSourse shader);
};

