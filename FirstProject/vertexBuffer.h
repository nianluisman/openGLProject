#pragma once
class vertexBuffer
{
private:
	unsigned int m_render;
public: 
	vertexBuffer(const void* data, unsigned int size);
	~vertexBuffer();
	void bind();
	void unbind();
};

