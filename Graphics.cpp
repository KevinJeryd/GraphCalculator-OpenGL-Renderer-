#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

int main(void)
{

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set the major version of OpenGL to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set the minor version of OpenGL to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set the OpenGL profile to core

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Learning", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); // IMPORTANT: Must be done so glew recognises OpenGL
    glViewport(0, 0, 800, 600); // Set the viewport to the whole window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// Set the callback function for when the window is resized
    glfwSwapInterval(1); // Enable vsync

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW ERROR" << std::endl;
    }



    
    {
        float vertices[] = { // The vertices of the square
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left 
        };

        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Enable alpha blending

        VertexArray va; // Create a vertex array
        VertexBuffer vb(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(3); // Push the position attribute
        layout.Push<float>(2); // Push the texture attribute

        va.AddBuffer(vb, layout); // Add the vertex buffer to the vertex array
        IndexBuffer ib(indices, sizeof(indices)/sizeof(unsigned int));

        glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        // Setup shader // TODO: Något problem med shader-klassen, saker runnar inte när jag instantiate:ar en shader
        Shader shader("res/shaders/Basic.shader"); // Create a shader
        shader.Bind(); // Bind the shader
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f); // Set the uniform variable in the shader (the color in this case)
        shader.SetUniform1i("u_Texture", 0); // Set the uniform variable in the shader (the texture in this case)
        shader.SetUniformMat4f("u_MVP", projectionMatrix);

        Texture texture("res/textures/Soraka.png");
        texture.Bind();

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer; // Create a renderer
        
        float r = 0.0f; // The red value of the color
        float increment = 0.05f; // The amount to increment the red value by

        //GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
        
            // Input
            processInput(window); // Check for input

            
            renderer.Clear(); // Clear the screen
            
            
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f); // Set the uniform variable in the shader (the color in this case)
            shader.Unbind();

            renderer.Draw(va, ib, shader); // Draw the vertex array

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // Draw the triangle
            

        
            if (r > 1.0f) // If the red value is greater than 1
			    increment = -0.05f; // Decrease the red value by 0.01 (this will make the triangle flash red and black
		    else if (r < 0.0f)
			    increment = 0.05f; // Increase the red value by 0.01

            r += increment; // Increment the red value


            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    glfwTerminate();
    return 0;
}