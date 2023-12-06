#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0)
{
	m_RendererID = CreateShader(ParseShader(filepath).VertexSource, ParseShader(filepath).FragmentSource);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {

    std::ifstream stream(filepath); // Open the file

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1 // The type of the shader, it will serve as the index of the source code in the string stream
    };

    std::string line; // The current line in the file
    std::stringstream ss[2]; // The source code of the shader (vertex and fragment)
    ShaderType type = ShaderType::NONE; // The type of the shader

    while (getline(stream, line)) { // While there are lines left in the file

        // If the line contains "#shader" then set the type of the shader that is currently read
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) { // If the line contains "vertex"
                type = ShaderType::VERTEX; // Set mode to vertex
            }
            else if (line.find("fragment") != std::string::npos) { // If the line contains "fragment"
                type = ShaderType::FRAGMENT; // Set mode to fragment
            }
        }
        else { // If the line isn't the type, then add the source code to the corresponding string stream
            ss[(int)type] << line << '\n';
        }
    }

    stream.close(); // Close the file

    return { ss[0].str(), ss[1].str() }; // Return the source code of the shader as a ShaderProgramSource struct
}

// Type - The type of the shader (vertex or fragment)
// Source - The source code of the shader (a string in this case)
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

    // glCreateShader creates a shader object and returns the id of the shader
    // A shader object is used to maintain the source code strings that define a shader. 
    GLCall(unsigned int id = glCreateShader(type));

    // Pointer to the first character in the string
    const char* src = source.c_str(); // Get the source code of the shader

    // glShaderSource sets the source code in the specific shader object with the id that is reffered, to the source code specified in the string (char*)
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id)); // Compile the shader with the id that is reffered

    // Error handling
    int result;

    // glGetShaderiv returns a parameter from a shader object, in this case the compile status of the shader, and stores it in the result variable
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) { // If the compilation failed
        int length; // The length of the error message
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // Get the length of the error message and store it in the length variable
        char* message = (char*)alloca(length * sizeof(char)); // Allocate memory on the stack for the error message, alloca kan allocate memory on the stack dynamically
        GLCall(glGetShaderInfoLog(id, length, &length, message)); // Get the error message and store it in the message variable
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl; // Print which shader failed to compile
        std::cout << message << std::endl; // Print the error message
        GLCall(glDeleteShader(id)); // Delete the shader because it failed to compile
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int program = glCreateProgram()); // Create a program (a program is a collection of shaders)
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); // Create a vertex shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); // Create a vertex shader

    GLCall(glAttachShader(program, vs)); // Attach the vertex shader to the program
    GLCall(glAttachShader(program, fs)); // Attach the fragment shader to the program
    GLCall(glLinkProgram(program)); // Link the program (similar to how c++ links code)
    GLCall(glValidateProgram(program)); // Validate the program

    GLCall(glDeleteShader(vs)); // Delete the vertex shader since it is now in the program (similar to how c++ generates an executable and you can delete the object files)
    GLCall(glDeleteShader(fs)); // Delete the fragment shader since it is now in the program

    return program;
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}
