#pragma once
class indexBuffer
{
private:
	unsigned int m_render;
	unsigned int m_count;
public:
	indexBuffer(const unsigned int* data, unsigned int count);
	~indexBuffer();
	void bind();
	void unbind();
	inline unsigned int getCount() { return m_count; }
};
