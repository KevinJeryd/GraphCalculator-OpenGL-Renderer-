#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"
#include <stdexcept>
#include <iostream>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false); // If the type is not one of the above, break
		return 0;
	}
};

inline std::ostream& operator<<(std::ostream& stream, const VertexBufferElement& element)
{
	stream << (element.type == GL_FLOAT ? "FLOAT" : element.type == GL_UNSIGNED_INT ? "UINT" : "BYTE") << " " << element.count << " " << element.normalized;
	return stream;
}

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) 
	{
	
	};

	// Varför använda templates istället för att overloada funktionen?
	template<typename T>
	void Push(unsigned int count)
	{
		std::runtime_error(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		VertexBufferElement VBE = { GL_FLOAT, count, GL_FALSE }; // Add the element to the vector

		m_Elements.push_back(VBE); // Add the element to the vector
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); // Add the size of the element to the stride
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE }); // Add the element to the vector
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT); // Add the size of the element to the stride
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE }); // Add the element to the vector
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE); // Add the size of the element to the stride
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }

	inline unsigned int GetStride() const { return m_Stride; }
};