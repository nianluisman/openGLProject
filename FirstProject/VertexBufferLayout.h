#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_element;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride( 0 ), m_element(std::vector<VertexBufferElement>()) {}

	template<typename T>
	void push(unsigned int count) {
		//static_assert(false);
	}
	template<>
	void push<float>(unsigned int count) {
		m_element.push_back({GL_FLOAT, count, false});
		m_Stride += sizeof(GLfloat);
	}
	template<>
	void push<unsigned int >(unsigned int count) {
		m_element.push_back({ GL_UNSIGNED_INT, count, false });
		m_Stride += sizeof(unsigned int);
	}
	template<>
	void push<unsigned char >(unsigned int count) {
		m_element.push_back({ GL_UNSIGNED_BYTE, count, true });
		m_Stride += sizeof(unsigned char);
	}
	inline std::vector<VertexBufferElement> getElement() const& { return m_element; }
	inline unsigned int getSride() const& { return m_Stride; }

};

