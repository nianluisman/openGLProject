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

#include "vertexBuffer.h"
#include "SimpleDraw.h"
#include "objectLoader.h"
#include "texureLoader.h"
#include "Shader.h"
#include "ComplexDraw.h"
#include "AnimatedObjects.h"
#include "Prism.h"
#include "cube.h"


//struct shaderPrograms {
//    unsigned int textureProgram;
//    unsigned int ColorProgram;
//};

// angle for rotating triangle
float angle = 0.0f;
unsigned int VAOCube, imageID_Floor, imageID_Wall, imageID_Stone, imageID_water, imageID_wood;
shaderPrograms shaderprogram;
unsigned int VAO_Treemodel, VBO_Treemodel, VAO_CarModel, VAO_spoon, VBO_spoon, VAO_Theapot, VBO_Theapot, VAO_table, VBO_table;
unsigned int VAO_Corwn, VAO_Monkey, VAO_donot;
unsigned int VBO_Carmodel, VBO_Crown, VBO_Monkey, VBO_donot;
std::vector<float> verticesTree , verticesCar, verticesSpoon, verticesTheapot, verticesTable, Vertices_Corwn, Vetices_monkey, verticesDonot;
const aiMesh* meshModel;
Prism prism;
cube cubeObj;

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
bool firstMouse = true;

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

void initBuffers() {
    objectLoader CarLoader = objectLoader(&VAO_CarModel, VBO_Carmodel);
    verticesCar = CarLoader.loadModel("objects/BMW_v3.obj");
    CarLoader.setBuffers(verticesCar, "Color");

    objectLoader SpoonLoader = objectLoader(&VAO_spoon, VBO_spoon);
    verticesSpoon = CarLoader.loadModel("objects/NewSpoon2.obj");
    SpoonLoader.setBuffers(verticesSpoon, "Texture");

    objectLoader TheapotLoader = objectLoader(&VAO_Theapot, VBO_Theapot);
    verticesTheapot = TheapotLoader.loadModel("objects/teapot.obj");
    TheapotLoader.setBuffers(verticesTheapot, "Color");

    objectLoader TableLoader = objectLoader(&VAO_table, VBO_table);
    verticesTable = TableLoader.loadModel("objects/Table.obj");
    TableLoader.setBuffers(verticesTable, "Color");

    objectLoader CrownLoader = objectLoader(&VAO_Corwn, VBO_Crown);
    Vertices_Corwn = CrownLoader.loadModel("objects/crown.obj");
    CrownLoader.setBuffers(Vertices_Corwn, "Color");

    objectLoader MonkeyLoader = objectLoader(&VAO_Monkey, VBO_Monkey);
    Vetices_monkey = MonkeyLoader.loadModel("objects/monkey.obj");
    MonkeyLoader.setBuffers(Vetices_monkey, "Color");

    objectLoader DonotLoader = objectLoader(&VAO_donot, VBO_donot);
    verticesDonot = DonotLoader.loadModel("objects/donot2.obj");
    DonotLoader.setBuffers(verticesDonot, "Texture");

    objectLoader TreeLoader = objectLoader(&VAO_Treemodel, VBO_Treemodel);
    verticesTree = TreeLoader.loadModel("objects/Tree1.3ds");
    TreeLoader.setBuffers(verticesTree, "Texture");

    prism = Prism();

    prism.setBuffer();

    cubeObj = cube();

    cubeObj.setBuffer();
}

float deltaTime = 0.0f;	
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
    ComplexDraw drawObject = ComplexDraw();
    AnimatedObjects animadedObject = AnimatedObjects();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);


    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    glUseProgram(shaderprogram.textureProgram);

    //set view for textures shader objects
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    
    drawObject.drawFLoor(cubeObj.VAOcube, shaderprogram, imageID_Floor, sizeof(cubeObj.vertices));
    drawObject.drawHouse(cubeObj.VAOcube, shaderprogram, imageID_wood, imageID_Wall, sizeof(cubeObj.vertices));
    drawObject.drawTent(prism.VAOprism, shaderprogram, 0, sizeof(prism.vertices));
    

    //set view for color shader objects
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(0);

    animadedObject.drawTheaPot(VAO_Theapot, shaderprogram, verticesTheapot.size());
    drawObject.drawTable(VAO_table, shaderprogram, verticesTable.size());
    

    drawObject.drawTrees(VAO_Treemodel, shaderprogram.textureProgram, imageID_Floor, verticesTree.size());
   
    drawObject.drawSpoon(VAO_spoon,shaderprogram,imageID_Stone,verticesSpoon.size());

    animadedObject.drawCrown(VAO_Corwn, shaderprogram, 0, Vertices_Corwn.size());

    drawObject.drawMonkey(VAO_Monkey, shaderprogram, 0, Vetices_monkey.size());

    animadedObject.drawDonot(VAO_donot, shaderprogram, imageID_water, verticesDonot.size());

    animadedObject.drawAndAnimateCar(VAO_CarModel, shaderprogram, verticesCar.size());

    glBindVertexArray(0);
  
    animadedObject.DrawAndAnimateLightPost(shaderprogram);
    glBindVertexArray(0);
    glutSwapBuffers();

}


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
    if (!glfwInit()) { // initialize GLFW
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    setupWindow(argc, argv);
    glutSetCursor(GLUT_CURSOR_NONE);
    
    Shader shader = Shader();
    ShaderProgramSourse shaderSource =  shader.parseShaders("basic.shader");
    shaderprogram = shader.creatShader(shaderSource);
   
    texureLoader textureload = texureLoader();
    bool sucsesFloor = textureload.loadTexture("Textures/Floor/aerial_rocks_02_diff_4k.jpg" ,&imageID_Floor);
 
    bool sucsesWall = textureload.loadTexture("Textures/wall/Tileable_Red_Brick_Texturise.jpg", &imageID_Wall);

    bool sucsesDoor = textureload.loadTexture("Textures/door/Plastic_004_basecolor.jpg", &imageID_Stone);

    bool sucsesWater = textureload.loadTexture("Textures/Water/Pool_Water_Texture_Diff.jpg", &imageID_water);

    bool sucsesWood = textureload.loadTexture("Textures/wood/Substance_Graph_BaseColor.jpg", &imageID_wood);

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
