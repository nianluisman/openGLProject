#include "Shader.h"


ShaderProgramSourse Shader::parseShaders(const std::string& filePath) {
    std::fstream stream(filePath);
    std::string line;
    std::stringstream ss[3];
    if (!stream.is_open()) {
        std::cout << "could not open file";
    }
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        TEXURESFRACMENT = 1,
        BASICFRACMENT = 2
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
        else {
            ss[(int)shaderType] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str(), ss[2].str() };
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
    unsigned int vs = -1;
    unsigned int tfs = -1;
    unsigned int cfs = -1;
    if (!shader.VertexProgram.empty()) {
        vs = CompileShader(GL_VERTEX_SHADER, shader.VertexProgram);
        glAttachShader(textureProgram, vs);
        glAttachShader(colorProgram, vs);
    }
    if (!shader.TexureFracmentProgram.empty()) {
        tfs = CompileShader(GL_FRAGMENT_SHADER, shader.TexureFracmentProgram);
        glAttachShader(textureProgram, tfs);
    }
    if (!shader.FracmentColorProgram.empty()) {
        cfs = CompileShader(GL_FRAGMENT_SHADER, shader.FracmentColorProgram);
        glAttachShader(colorProgram, cfs);
    }

    glLinkProgram(textureProgram);
    glValidateProgram(textureProgram);

    glLinkProgram(colorProgram);
    glValidateProgram(colorProgram);


    int success;
    glGetProgramiv(textureProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(colorProgram, GL_LINK_STATUS, &success);
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

    return { textureProgram, colorProgram };
}