#include "ComplexDraw.h"

void  ComplexDraw::drawTrees(unsigned int VAO, unsigned int shaderprogram, unsigned int texture_id, int size) {
    // Set the model matrix for the first cube

       // Set the model matrix for the first cube
    glUseProgram(shaderprogram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, -.5f, -4.0f));
    model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, size);

    float offsetTree = 10.0f;
    for (int i = 0; i < 5; i++) {
        model = glm::translate(model, glm::vec3(offsetTree, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, size);
        offsetTree += 5.0f;
    }

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, -.5f, -5.0f));
    model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, size);

    for (int i = 0; i < 4; i++) {
        model = glm::translate(model, glm::vec3(10, 0.0f, 0.0f)); // Translate to the right
        glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, size);
        offsetTree += 5.0f;
    }


    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, -.5f, 5.0f)); // Translate to the right
    model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    for (int i = 0; i < 5; i++) {
        model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -0.0f)); // Translate to the right
        glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, size);
        offsetTree -= 5.0f;
    }

}

void ComplexDraw::drawHouse(unsigned int VAO, shaderPrograms shaderprogram, unsigned int imageID_Door, unsigned int imageID_Wall, int size) {
    glBindVertexArray(VAO);
    SimpleDraw draw2 = SimpleDraw();
    glm::mat4 model = glm::mat4(1.0f);
    glBindTexture(GL_TEXTURE_2D, imageID_Wall);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, imageID_Door);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.0f, 0.3f));
    model = glm::scale(model, glm::vec3(0.5f, 0.9f, 0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);
    GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
    glUniform4f(colorLocation, .52, 0.27, .08, 0.0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5, 0.5, 1));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glPushMatrix();
    draw2.drawPyramid();
    glPopMatrix();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.2f, 0.8f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3, 0.8, 0.3));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glPushMatrix();
    glUniform4f(colorLocation, 0.0f, 1.0f, 0.0, 0.0);
    draw2.drawCylinder();
    model = glm::translate(model, glm::vec3(20.0f, 0.8f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glutPostRedisplay();
    glPopMatrix();
}

void ComplexDraw::drawFLoor(unsigned int VAO, shaderPrograms shaderprogram, unsigned int texture_id, int size) {
    glUseProgram(0);
    glUseProgram(shaderprogram.textureProgram);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(15, .1, 15));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glutPostRedisplay();

}

void ComplexDraw::drawTable(unsigned int VAO, shaderPrograms shaderprogram, int size) {
    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);
    glBindVertexArray(VAO);
    GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
    glUniform4f(colorLocation, 0.5f, 0.5f, 0.5f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, -0.5f, 3.0f));
    model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, size);
}

void ComplexDraw::drawSpoon(unsigned int VAO, shaderPrograms shaderprogram, unsigned int text_id, int size) {
    glUseProgram(0);
    glUseProgram(shaderprogram.textureProgram);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, text_id);
    //GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
    //glUniform4f(colorLocation, 0.5f, 0.5f, 0.5f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, 0.5f, -3.0f));
    model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0));
    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, size);
}

void ComplexDraw::drawMonkey(unsigned int VAO, shaderPrograms shaderprogram, unsigned int text_id, int size) {
    glUseProgram(0);
    glUseProgram(shaderprogram.ColorProgram);
    glBindVertexArray(VAO);
    GLint colorLocation = glGetUniformLocation(shaderprogram.ColorProgram, "color");
    glUniform4f(colorLocation, 0.0f, 0.5f, 0.5f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0f, 0.5f, 3.0f));
    model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.ColorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, size);
}

//void ComplexDraw::drawDonot(unsigned int VAO, shaderPrograms shaderprogram, unsigned int text_id, int size) {
//    glUseProgram(0);
//    glUseProgram(shaderprogram.textureProgram);
//    glBindVertexArray(VAO);
//    glBindTexture(GL_TEXTURE_2D, text_id);
//    GLint colorLocation = glGetUniformLocation(shaderprogram.textureProgram, "color");
//    glUniform4f(colorLocation, 0.5f, 0.0f, 0.5f, 1.0f);
//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::translate(model, glm::vec3(6.0f, 0.5f, 3.0f));
//    model = glm::scale(model, glm::vec3(.5, 0.5, .5));
//    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    glUniformMatrix4fv(glGetUniformLocation(shaderprogram.textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//    glDrawArrays(GL_TRIANGLES, 0, size);
//}