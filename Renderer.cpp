#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR); // While there are errors, clear them
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) { // Checking and assigning at the same time
        std::cout << "[OpenGL Error] (" << error << ").\nFunction: " << function << " in " << file << " on line: " << line << std::endl; // Print the error
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind(); // Bind the shader
	va.Bind(); // Bind the vertex array
	ib.Bind(); // Bind the index buffer
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // Draw the elements
}

void Renderer::Clear() const {
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 0.1f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT)); // Clears the screen every frame so that the previous frame is not visible
}