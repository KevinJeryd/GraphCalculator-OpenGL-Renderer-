#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	: m_Count(count)
{
	ASSERT (sizeof(unsigned int) == sizeof(GLuint)); // If the size of an unsigned int is not the same as the size of a GLuint, break
	GLCall(glGenBuffers(1, &m_RendererID)); // Generate a buffer, 1 is the number of buffers, and &m_RendererID is the address of the buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW)); // Set the data of the buffer, GL_STATIC_DRAW is the usage of the buffer
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); // Delete the buffer, 1 is the number of buffers, and &m_RendererID is the address of the buffer
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
