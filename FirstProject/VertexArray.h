#pragma once
#include "vertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
private:

public:
	VertexArray();
	~VertexArray();
	void addBuffer(const vertexBuffer buffer, const VertexBufferLayout& layout);
};

