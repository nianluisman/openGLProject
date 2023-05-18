#include "Shader.h"


ShaderProgramSourse Shader::parseShaders(const std::string& filePath) {
    std::fstream stream(filePath);
    std::string line;
    std::stringstream ss[4];
    if (!stream.is_open()) {
        std::cout << "could not open file";
    }
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        TEXURESFRACMENT = 1,
        BASICFRACMENT = 2,
        SHINYFRACMENT = 3
    };
    ShaderType shaderType = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#vertex_shader") != std::string::npos) {
            shaderType = ShaderType::VERTEX;
        }
        else if (line.find("#basic_color_shader") != std::string::npos) {
            shaderType = ShaderType::BASICFRACMENT;
        }
        else if (line.find("#texture_fracment_shader") != std::string::npos) {
            shaderType = ShaderType::TEXURESFRACMENT;
        }
        else if (line.find("#shiny_fracment_shader") != std::string::npos) {
            shaderType = ShaderType::SHINYFRACMENT;
        }
        else {
            ss[(int)shaderType] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str(), ss[2].str(), ss[3].str()};
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& sourse) {
    unsigned int id = glCreateShader(type);
    const char* src = sourse.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Falled to compile " << (type == GL_VERTEX_SHADER ? "vertex shader" : "fracment shader") << std::endl;
        std::cout << message << std::endl;
        return 0;
    }
    return id;
}

shaderPrograms Shader::creatShader(const ShaderProgramSourse shader) {
    unsigned int textureProgram = glCreateProgram();
    unsigned int colorProgram = glCreateProgram();
    unsigned int shinyProgram = glCreateProgram();
    unsigned int vs = -1;
    unsigned int tfs = -1;
    unsigned int cfs = -1;
    unsigned int sfs = -1;
    if (!shader.VertexSourse.empty()) {
        vs = CompileShader(GL_VERTEX_SHADER, shader.VertexSourse);
        glAttachShader(textureProgram, vs);
        glAttachShader(colorProgram, vs);
        glAttachShader(shinyProgram, vs);
    }
    if (!shader.TexureFracmentSourse.empty()) {
        tfs = CompileShader(GL_FRAGMENT_SHADER, shader.TexureFracmentSourse);
        glAttachShader(textureProgram, tfs);
    }
    if (!shader.FracmentColorSourse.empty()) {
        cfs = CompileShader(GL_FRAGMENT_SHADER, shader.FracmentColorSourse);
        glAttachShader(colorProgram, cfs);
    }
    if (!shader.ShinyShaderSourse.empty()) {
        cfs = CompileShader(GL_FRAGMENT_SHADER, shader.ShinyShaderSourse);
        glAttachShader(shinyProgram, sfs);
    }

    glLinkProgram(textureProgram);
    glValidateProgram(textureProgram);

    glLinkProgram(colorProgram);
    glValidateProgram(colorProgram);

    glLinkProgram(shinyProgram);
    glValidateProgram(shinyProgram);

    int success;
    glGetProgramiv(textureProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(colorProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(shinyProgram, GL_LINK_STATUS, &success);
    if (!success) {
        int logLength;
        glGetProgramiv(textureProgram, GL_INFO_LOG_LENGTH, &logLength);

        char* infoLog = (char*)alloca(logLength * sizeof(char));
        glGetProgramInfoLog(textureProgram, logLength, NULL, infoLog);

        std::string printableInfoLog;
        for (int i = 0; i < logLength; i++) {
            if (isprint(infoLog[i])) {
                printableInfoLog += infoLog[i];
            }
        }

        std::cout << "Program failed: " << printableInfoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(tfs);
    glDeleteShader(cfs);

    return { textureProgram, colorProgram, shinyProgram };
}