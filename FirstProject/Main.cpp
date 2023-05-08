/*
* name: nian luisman
* student number: 1194178
*/

#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2/SOIL2.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "objloader.h"

#include "vertexBuffer.h"
#include "SimpleDraw.h"
#include "objectLoader.h"
#include "texureLoader.h"

struct shaderPrograms {
    unsigned int textureProgram;
    unsigned int ColorProgram;
};

// angle for rotating triangle
float angle = 0.0f;
unsigned int shader, VAOCube, imageID_Floor, imageID_Wall, imageID_Door;
shaderPrograms shaderprogram;
unsigned int VAO_Treemodel, VBO_Treemodel, VAO_CarModel, VAO_Theapot, VBO_Theapot;
std::vector<float> verticesTree , verticesCar, verticesTheapot;
const aiMesh* meshModel;

// Define global variables for the camera position and rotation
float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;
float camRotY = 0.0f;

glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                             glm::vec3(-2.0f, -2.0f, -2.0f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 projection;


// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


glm::vec3 modelPos(0.0f, 0.0f, -1.0f); // initial model position

glm::mat4 model = glm::mat4(1.0f); // identity matrix for model orientation
//glm::mat4 view = glm::lookAt(cameraPos, modelPos, glm::vec3(0.0f, 1.0f, 0.0f)); // initial view matrix
glm::vec3 relativePos;
glm::mat4 cameraTranslation;

int windowWidth = 1300;
int windowHeight = 1000;



float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;


float carMoveNum = 0.0f;
float switchCar = false;
float colorNum = 0.0;
float switchColor = false;


// Define the vertex data
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
};

float vertices[] = {
    // position              color            texCoord
      -0.5f, -0.5f, -0.5f     ,1.0f, 0.0f, 0.0f    ,0.0f, 0.0f, //vertex 0 
      0.5f, -0.5f, -0.5f      ,0.0f, 1.0f, 0.0f    ,1.0f, 0.0f,
      0.5f,  0.5f, -0.5f      ,0.0f, 0.0f, 1.0f    ,1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f     ,1.0f, 1.0f, 0.0f    ,0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f     ,0.0f, 1.0f, 1.0f    ,1.0f, 0.0f,
      0.5f, -0.5f,  0.5f      ,1.0f, 0.0f, 1.0f    ,0.0f, 0.0f,
      0.5f,  0.5f,  0.5f      ,0.5f, 0.5f, 0.5f    ,0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f     ,1.0f, 1.0f, 1.0f    ,1.0f, 1.0f
};

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
    std::string TexureFracmentProgram;
    std::string FracmentColorProgram;
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

shaderPrograms creatShader(const ShaderProgramSourse shader){
    unsigned int textureProgram =  glCreateProgram();
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
void setupWindow(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("NianLuisman eindproject ");

    glewInit();
}
std::vector<glm::vec3> out_vertices = std::vector<glm::vec3>();
std::vector<glm::vec2> out_uvs = std::vector<glm::vec2>();
std::vector<glm::vec3> out_normals = std::vector<glm::vec3>();

std::vector<glm::vec3> car_out_vertices = std::vector<glm::vec3>();
std::vector<glm::vec2> car_out_uvs = std::vector<glm::vec2>();
std::vector<glm::vec3> car_out_normals = std::vector<glm::vec3>();
std::vector<unsigned short> car_indecs = std::vector<unsigned short>();
void initBuffers() {

    objectLoader objLoader = objectLoader();
    //set VAO
    unsigned int VBO_Carmodel;

    glBindVertexArray(0);

    verticesCar = objLoader.loadModel("objects/BMW_v3.obj");
   // loadOBJ("objects/BMW_v3.obj", car_out_vertices, car_out_uvs, car_out_normals);
   // loadAssImp("objects/BMW_v3.obj", car_indecs, car_out_vertices, car_out_uvs, car_out_normals);
    glGenVertexArrays(1, &VAO_CarModel);
    glBindVertexArray(VAO_CarModel);
    glGenBuffers(1, &VBO_Carmodel);

    //// Bind the VBO and pass the vertex data to it
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Carmodel);
    glBufferData(GL_ARRAY_BUFFER, verticesCar.size() * sizeof(GLfloat), &verticesCar.front(), GL_STATIC_DRAW);

    //// Specify the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //// Enable the vertex attributes
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    //verticesTheapot = objLoader.loadModel("objects/teapot.obj");
    loadOBJ("objects/teapot.obj", out_vertices, out_uvs, out_normals);

    glGenVertexArrays(1, &VAO_Theapot);
    glBindVertexArray(VAO_Theapot);
    glGenBuffers(1, &VBO_Theapot);
    //// Bind the VBO and pass the vertex data to it
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Theapot);
    glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices.front(), GL_STATIC_DRAW);

    //uto positonLocation = glGetUniformLocation(shaderprogram.textureProgram, "aPos");

    //// Specify the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    verticesTree = objLoader.loadModel("objects/Tree1.3ds");
    

    glGenVertexArrays(1, &VAO_Treemodel);
    glBindVertexArray(VAO_Treemodel);

    glGenBuffers(1, &VBO_Treemodel);
    //// Bind the VBO and pass the vertex data to it
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Treemodel);
    glBufferData(GL_ARRAY_BUFFER, verticesTree.size() * sizeof(float), &verticesTree[0], GL_STATIC_DRAW);

    //// Specify the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    

    
    vertexBuffer vb = vertexBuffer(vertices, sizeof(vertices));
    unsigned int VBOWall, EBOWall;
    glGenBuffers(1, &VBOWall);//make data buffer
    glGenBuffers(1, &EBOWall);// index buffer 
    glGenVertexArrays(1, &VAOCube);// vertex array buffer

    glBindVertexArray(VAOCube);

    glBindBuffer(GL_ARRAY_BUFFER, VBOWall);//bind to array buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWall);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //handel some models
    //load tree




   // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //// Enable the vertex attributes
    //glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  
}
void  drawTrees() {
    // Set the model matrix for the first cube
    glm::mat4 model = glm::mat4(1.0f);

    glBindTexture(GL_TEXTURE_2D, imageID_Floor);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, -.5f, -4.0f));
    model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, verticesTree.size());

    float offsetTree = 10.0f;
    for (int i = 0; i < 5; i++) {
        model = glm::translate(model, glm::vec3(offsetTree, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, verticesTree.size());
        offsetTree += 5.0f;
    }

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, -.5f, -5.0f));
    model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, verticesTree.size() );

    for (int i = 0; i < 4; i++) {
        model = glm::translate(model, glm::vec3(10, 0.0f, 0.0f)); // Translate to the right
        glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, verticesTree.size());
        offsetTree += 5.0f;
    }


    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, -.5f, 5.0f)); // Translate to the right
    model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    for (int i = 0; i < 5; i++) {
        model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -0.0f)); // Translate to the right
        glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, verticesTree.size() );
        offsetTree -= 5.0f;
    }
    
}

void drawAndAnimateCar() {
    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);
  //  glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");

    glUniform4f(colorLocation, 1.0f, 0.0f, 0.0, 0.0);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.f, -.45f, carMoveNum));
    model = glm::scale(model, glm::vec3(0.005, 0.005, 0.005));

    if (switchCar) {
        carMoveNum += 0.01f;
        if (carMoveNum >= 2.0) {
            switchCar = false;
        }
    }
    else {
        carMoveNum -= 0.01f;
        if (carMoveNum <= -2.0) {
            switchCar = true;
        }
    }
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
   // glDrawElements(GL_TRIANGLES, sizeof(verticesCar.size()) * sizeof(GLfloat), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES,0 , verticesCar.size());


}
void drawHouse() {
    Draw* draw = new Draw();
    glm::mat4 model = glm::mat4(1.0f);
    glBindTexture(GL_TEXTURE_2D, imageID_Wall);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.0f, 0.0f)); 
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, imageID_Door);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.0f, 0.3f)); 
    model = glm::scale(model, glm::vec3(0.5f, 0.9f, 0.5f)); 
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLE_FAN, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);
    GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
    glUniform4f(colorLocation, .52, 0.27, .08, 0.0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.f, 0.0f)); 
    model = glm::scale(model, glm::vec3(0.5, 0.5, 1));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glPushMatrix();
    draw->drawPyramid();
    glPopMatrix();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.2f, 0.8f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3, 0.8, 0.3));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glPushMatrix();
    glUniform4f(colorLocation, 0.0f, 1.0f, 0.0, 0.0);
    draw->drawCylinder();
    model = glm::translate(model, glm::vec3(20.0f, 0.8f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glutPostRedisplay();
    glPopMatrix();

}
void drawTheaPot() {
    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);
    GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
    glUniform4f(colorLocation, 0.0f, 1.0f, 0.0, 0.0);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.0f,0.0f ));
    model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
    glDrawArrays(GL_TRIANGLES, 0, out_vertices.size());
}
void drawFLoor() {
    // Set the model matrix for the first cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f)); // Translate to the left
    model = glm::scale(model, glm::vec3(15, .1, 15));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glutPostRedisplay();
}

void DrawAndAnimateLightPost() {
    Draw* draw = new Draw();
    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);

    GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
    glUniform4f(colorLocation, colorNum - .2f, colorNum, 0.0, 0.0);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, .4f, 0.8f)); // Translate to the right
    model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glPushMatrix();
    draw->drawSphere(1.0, 50, 50);
    glPopMatrix();

    glUniform4f(colorLocation, .52, 0.27, .07, 0.0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, -0.5f, 0.8f)); // Translate to the right
    model = glm::scale(model, glm::vec3(0.1, 0.8, 0.1));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glPushMatrix();
    draw->drawCylinder();
    glPopMatrix();

    if (switchColor) {
        colorNum += 0.01f;
        if (colorNum >= 0.8) {
            switchColor = false;
        }
    }
    else {
        colorNum -= 0.01f;
        if (colorNum <= 0.0) {
            switchColor = true;
        }
    }
}
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
void viewkeyboard(unsigned char key, int x, int y) {
    float cameraSpeed = 0.5f;
    if (key == 'w')
        cameraPos += cameraSpeed * cameraFront;
    if (key == 's')
        cameraPos -= cameraSpeed * cameraFront;
    if (key == 'a')
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (key == 'd')
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}
void renderScene(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);


    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    glUseProgram(shaderprogram.textureProgram);

    //set view for textures shader objects
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindTexture(GL_TEXTURE_2D, imageID_Floor);
    glBindVertexArray(VAOCube);

    drawFLoor();
    drawHouse();
    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);
    glBindVertexArray(VAO_Theapot);
    drawTheaPot();

    //set view for color shader objects
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(0);

    glUseProgram(shaderprogram.textureProgram);
    glBindVertexArray(VAO_Treemodel);

    drawTrees();
   glBindVertexArray(VAO_CarModel);

    drawAndAnimateCar();

    glBindVertexArray(0);
    DrawAndAnimateLightPost();

    glBindVertexArray(0);
    glutSwapBuffers();

}

bool firstMouse = true;
void mouse_callback(int xposIn, int yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset =  xpos -  lastX ;
    float yoffset = lastY - ypos; 

    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    lastX = xpos;
    lastY = ypos;
    glutWarpPointer(windowWidth / 2, windowHeight / 2);
    lastX = windowWidth / 2;
    lastY = windowHeight / 2;
}

int main(int argc, char** argv) {

    setupWindow(argc, argv);
    glutSetCursor(GLUT_CURSOR_NONE);
    
    ShaderProgramSourse shaderProgram = parseShaders("basic.shader");
    shaderprogram = creatShader(shaderProgram);
    unsigned int id;
    texureLoader textureload = texureLoader();
    bool sucsesFloor = textureload.loadTexture("Textures/Floor/aerial_rocks_02_diff_4k.jpg" ,&imageID_Floor);
 
    bool sucsesWall = textureload.loadTexture("Textures/wall/Tileable_Red_Brick_Texturise.jpg", &imageID_Wall);

    bool sucsesDoor = textureload.loadTexture("Textures/door/Plastic_004_basecolor.jpg", &imageID_Door);

    initBuffers();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(viewkeyboard);
    glutMotionFunc(mouse_callback);
    glutMainLoop();

    return 1;
}
