#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include <iostream>


VertexArray::VertexArray() 
	: m_RendererID(0) 
{
	GLCall(glGenVertexArrays(1, &m_RendererID)); // Generate 1 vertex array objects and store the IDs in VAO
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID)); // Delete the vertex array object
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// First bind the vertex array, then bind the buffer we want to set up the layout for, then set up the layout
	Bind();

	vb.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements(); // Get the elements from the layout

	unsigned int offset = 0; // Set the offset to 0

	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i]; // Get the element from the elements

		GLCall(glEnableVertexAttribArray(i)); // Enable the vertex attribute array
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset)); // Tell OpenGL how to interpret the data in the vertex buffer
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID)); // Bind the vertex array object
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0)); // Unbind the vertex array object
}
