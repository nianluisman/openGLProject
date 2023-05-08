#include "SimpleDraw.h"


void Draw::drawSphere(GLfloat radius, GLint slices, GLint stacks) {
    for (int i = 0; i < slices; i++) {
        GLfloat theta1 = i * 2.0 * M_PI / slices;
        GLfloat theta2 = (i + 1) * 2.0 * M_PI / slices;

        for (int j = 0; j < stacks; j++) {
            GLfloat phi1 = j * M_PI / stacks;
            GLfloat phi2 = (j + 1) * M_PI / stacks;

            // Compute the vertices of the quad
            GLfloat x1 = radius * sin(phi1) * cos(theta1);
            GLfloat y1 = radius * sin(phi1) * sin(theta1);
            GLfloat z1 = radius * cos(phi1);

            GLfloat x2 = radius * sin(phi1) * cos(theta2);
            GLfloat y2 = radius * sin(phi1) * sin(theta2);
            GLfloat z2 = radius * cos(phi1);

            GLfloat x3 = radius * sin(phi2) * cos(theta2);
            GLfloat y3 = radius * sin(phi2) * sin(theta2);
            GLfloat z3 = radius * cos(phi2);

            GLfloat x4 = radius * sin(phi2) * cos(theta1);
            GLfloat y4 = radius * sin(phi2) * sin(theta1);
            GLfloat z4 = radius * cos(phi2);

            // Draw the quad using glBegin(GL_QUADS) and glVertex3f
            glBegin(GL_QUADS);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glVertex3f(x3, y3, z3);
            glVertex3f(x4, y4, z4);
            glEnd();
        }
    }
}

void Draw::drawCube() {
    // Front Face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
}

void Draw::drawCylinder() {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 1.0f, 0.0f); // Apex of the cone
    for (int i = 0; i <= 360; i += 10) {
        float angle = (float)i * (float)M_PI / 180.0f;
        glVertex3f(0.5f * sin(angle), 1.0f, 0.5f * cos(angle)); // Points on the base of the cone
    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 360; i += 10) {
        float angle = (float)i * (float)M_PI / 180.0f;
        glVertex3f(0.5f * sin(angle), 1.0f, 0.5f * cos(angle)); // Points on the top of the cylinder
        glVertex3f(0.5f * sin(angle), 0.0f, 0.5f * cos(angle)); // Points on the bottom of the cylinder
    }
    glEnd();
}

void Draw::drawTower(float height, float slices, float coneHeight, float radius, float coneRadius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, height, 0.0f);
    for (int i = 0; i <= slices; ++i) {
        float angle = i * 2.0f * M_PI / slices;
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        glVertex3f(x, height - coneHeight, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, height - coneHeight, 0.0f);
    for (int i = 0; i <= slices; ++i) {
        float angle = i * 2.0f * M_PI / slices;
        float x = cos(angle) * coneRadius;
        float z = sin(angle) * coneRadius;
        glVertex3f(x, height, z);
    }
    glEnd();
}

void Draw::drawPyramid() {
    // Draw the front face
    glBegin(GL_TRIANGLES);
    glColor3f(0.1f, 0.5f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // Draw the right face
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // Draw the back face
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // Draw the left face
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    // Draw the base
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

}
