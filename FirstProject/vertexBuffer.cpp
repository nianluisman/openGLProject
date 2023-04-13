#include "vertexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

vertexBuffer::vertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &m_render);
    glBindBuffer(GL_ARRAY_BUFFER, m_render);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
vertexBuffer::~vertexBuffer() {
    glDeleteBuffers(1, &m_render);
}

void vertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_render);
}
void vertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
