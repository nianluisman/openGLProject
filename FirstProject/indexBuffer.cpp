#include "indexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

indexBuffer::indexBuffer(const unsigned int* data, unsigned int count)
    : m_count{count} {
    glGenBuffers(1, &m_render);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_render);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count* sizeof(unsigned int), data, GL_STATIC_DRAW);
}
indexBuffer::~indexBuffer() {
    glDeleteBuffers(1, &m_render);
}

void indexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_render);
}
void indexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
