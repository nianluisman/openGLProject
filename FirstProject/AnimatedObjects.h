#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "SimpleDraw.h"

float carMoveNum = 0.0f;
float switchCar = false;
float colorNum = 0.0;
float switchColor = false;


float angleTheapot = 0.0f; // initial angle
float rotationSpeed = glm::radians(45.0f); // rotation speed in radians per second
float distance = 1.0f; // distance of translation in x-axis
float x = 0.0f; // current x-position of the model
float lastTime = 0.0f;

class AnimatedObjects
{
private:

public:
    void drawAndAnimateCar(unsigned int VAO, shaderPrograms shaderProgram, int size) {
        glBindVertexArray(VAO);
        glUseProgram(0);
        glUseProgram(shaderProgram.ColorProgram);

        GLint colorLocation = glGetUniformLocation(shaderProgram.ColorProgram, "color");

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
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, size);
    }

  

    void drawTheaPot(unsigned int VAO, shaderPrograms shaderprogram, int size) {
        glUseProgram(0);
        glUseProgram(shaderprogram.ColorProgram);
        glBindVertexArray(VAO);
        GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
        glUniform4f(colorLocation, 1.0f, 0.843f, 0.0f, 1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.f, 0.1f, 3.0f));
        model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        angleTheapot = fmod(angleTheapot + rotationSpeed * deltaTime, glm::radians(720.0f));
        x = distance * sin(angleTheapot);

        model = glm::rotate(model, angleTheapot, glm::vec3(0.0, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(x, 0.0f, 0.0f));


        glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, size);
    }

    void DrawAndAnimateLightPost(shaderPrograms shaderprogram) {
        SimpleDraw* draw = new SimpleDraw();
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
};

