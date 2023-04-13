#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2/SOIL2.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "bufferHandler.h"
#include "Draw.h"

// angle for rotating triangle
float angle = 0.0f;
unsigned int shader, VAO, VBO, EBO, locationColor, imageID;

// Define global variables for the camera position and rotation
float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;
float camRotY = 0.0f;

glm::mat4 view, projection;


void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

struct ShaderProgramSourse {
    /*Programs read form the basic.shader file*/
    std::string VertexProgram;
    std::string FracmentProgram;
    std::string TexureFracmentProgram;
};

static ShaderProgramSourse parseShaders(const std::string& filePath) {
    std::fstream stream(filePath);
    std::string line;
    std::stringstream ss[3];
    if (!stream.is_open()) {
        std::cout << "could not open file";
    }
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        BASICFRACMENT = 1,
        TEXURESFRACMENT = 2
    };
    ShaderType shaderType = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#vertex_shader") != std::string::npos) {
            shaderType = ShaderType::VERTEX;
        }
        else if (line.find("#basic_fracment_shader") != std::string::npos) {
            shaderType = ShaderType::BASICFRACMENT;
        }
        else if (line.find("#texure_fracment_shader") != std::string::npos) {
            shaderType = ShaderType::TEXURESFRACMENT;
        }
        else {
            ss[(int)shaderType] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str(), ss[2].str()};
}


static unsigned int CompileShader(unsigned int type, const std::string& sourse) {
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
static int creatShader(const ShaderProgramSourse shader){
    unsigned int program =  glCreateProgram();
    unsigned int vs = -1;
    unsigned int fs = -1;
    if (!shader.VertexProgram.empty()) {
        vs = CompileShader(GL_VERTEX_SHADER, shader.VertexProgram);
        glAttachShader(program, vs);
    }
    if (!shader.FracmentProgram.empty()) {
    fs = CompileShader(GL_FRAGMENT_SHADER, shader.FracmentProgram);
    glAttachShader(program, fs);
    }

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        char* infoLog = (char*)alloca(logLength * sizeof(char));
        glGetProgramInfoLog(program, logLength, NULL, infoLog);

        std::string printableInfoLog;
        for (int i = 0; i < logLength; i++) {
            if (isprint(infoLog[i])) {
                printableInfoLog += infoLog[i];
            }
        }

        std::cout << "Program failed: " << printableInfoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
void viewkeyboard(unsigned char key, int x, int y) {
    // Update the view matrix based on user input from the keyboard
    float cameraSpeed = 0.05f; // adjust this as needed
    float angle = 1.0f;
        switch (key)
        {
        case 'w':
            view = glm::translate(view, cameraSpeed * glm::vec3(0.0f, 1.0f, -0.0f));
            break;
        case 's':
            view = glm::translate(view, cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f));
            break;
        case 'a':
            view = glm::translate(view, cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f));
            break;
        case 'd':
            view = glm::translate(view, cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 'q':
            view = glm::rotate(view, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 'e':
            view = glm::rotate(view, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case '1':
            view = glm::rotate(view, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
            break;
        case '3':
            view = glm::rotate(view, glm::radians(angle), glm::vec3(0.0f, 0.0f, -1.0f));
            break;
        }
    if (key == 27)
        exit(0);
    // Update the view matrix in the shader

    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        camZ -= 0.1f;
        break;
    case 's':
        camZ += 0.1f;
        break;
    case 'a':
        camX -= 0.1f;
        break;
    case 'd':
        camX += 0.1f;
        break;
    case 'q':
        camY += 0.1f;
        break;
    case 'e':
        camY -= 0.1f;
        break;
    case '1':
        camRotY += 1.0f;
        break;
    case '3':
        camRotY -= 1.0f;
        break;
    }
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

unsigned char* image;
bool loadTexture(const char* filename, unsigned int* textureID) {
    if (!filename || !textureID) {
        return false;
    }
    int width, height, channels;
    image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGB);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    *textureID = texID;
    return true;
}
// Define the vertex data
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
};

float vertices[] = {
    // position              color            texCoord
    -0.5f, -0.5f, -0.5f     ,1.0f, 0.0f, 0.0f    ,0.0f, 0.0f,
    0.5f, -0.5f, -0.5f      ,0.0f, 1.0f, 0.0f    ,1.0f, 0.0f,
    0.5f,  0.5f, -0.5f      ,0.0f, 0.0f, 1.0f    ,1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f     ,1.0f, 1.0f, 0.0f    ,0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f    ,0.0f, 1.0f, 1.0f    ,0.0f, 0.0f,
    0.5f, -0.5f,  0.5f     ,1.0f, 0.0f, 1.0f    ,1.0f, 0.0f,
    0.5f,  0.5f,  0.5f     ,0.5f, 0.5f, 0.5f    ,1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f    ,1.0f, 1.0f, 1.0f    ,0.0f, 1.0f
};

// Define the index data
GLuint indices[] = {
    0, 1, 2, // front
    2, 3, 0,
    1, 5, 6, // right
    6, 2, 1,
    7, 6, 5, // back
    5, 4, 7,
    4, 0, 3, // left
    3, 7, 4,
    4, 5, 1, // bottom
    1, 0, 4,
    3, 2, 6, // top
    6, 7, 3
};

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    Draw* draw = new Draw();

    glMatrixMode(GL_MODELVIEW);
    glUseProgram(shader);
    // Set the view and projection matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glBindTexture(GL_TEXTURE_2D, imageID);

    glBindVertexArray(VAO);

    // Set the model matrix for the first cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f)); // Translate to the left
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    // Set the model matrix for the second cube
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Translate to the right
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);


   // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glutSwapBuffers();
}

void changeSize(int w, int h) {

    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;

 
    glMatrixMode(GL_PROJECTION);
 
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {

    int windowWidth = 1300;
    int windowHeight = 1000;
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lighthouse3D- GLUT Tutorial");

    glewInit();
    
    ShaderProgramSourse shaderProgram = parseShaders("basic.shader");
    shader = creatShader(shaderProgram);
    unsigned int id;
    bool check = loadTexture("Textures/Floor/aerial_rocks_02_diff_4k.jpg" ,&imageID);
    if (check == false) {
        std::cout << "Texture loading whent wrong" << std::endl;
    }
    //set buffer
    vertexBuffer vb = vertexBuffer(vertices, sizeof(vertices));
    //set VAO
    glGenBuffers(1, &VBO);//make data buffer
    glGenBuffers(1, &EBO);// index buffer 
    glGenVertexArrays(1, &VAO);// vertex array buffer

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);//bind to array buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Define the view matrix
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),  // camera position
        glm::vec3(0.0f, 0.0f, 0.0f),  // camera target
        glm::vec3(0.0f, 0.0f, 0.0f)); // up vector

    // Define the projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),  // field of view
        (float)windowWidth / (float)windowHeight, // aspect ratio
        0.1f, 200.0f); // near and far planes

    // Set the view and projection matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(viewkeyboard);

    glutMainLoop();

    return 1;
}
