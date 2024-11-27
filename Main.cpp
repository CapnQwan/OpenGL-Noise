#include<iostream>
#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<stb\stb_image.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"



// Vertices coordinates
GLfloat vertices[] =
{/*
 //             COORDINATES                           /      COLORS            //
	-0.5f,       -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f,  0.3f,  0.02f,   // Lower left corner
	0.5f,        -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f,  0.3f,  0.02f,   // Lower right corner
	0.0f,        0.5f * float(sqrt(3)) * 2 / 3,  0.0f,  1.0f,  0.6f,  0.32f,   // Upper corner
	-0.25f,      0.5f * float(sqrt(3)) / 6,      0.0f,  0.9f,  0.45f,  0.17f,  // Inner Left
	0.25f,       0.5f * float(sqrt(3)) / 6,      0.0f,  0.9f,  0.45f,  0.17f,  // Inner Right
	0.0f,        -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f,  0.3f,  0.02f,   // Inner Down
 */
 //             COORDINATES                           /      COLORS            //
	-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Lower Left Corner
	-0.5f, 0.5f,  0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Upper Left Corner
	0.5f,  0.5f,  0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Upper Right corner
	0.5f, -0.5f,  0.0f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Lower Right COrner
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

int main()
{
	// Initialize GLFW
	glfwInit();



	// Tell GLFW what Version of OpenGL we are using
	// In this case we are using 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create a GLFWwindow object of 800 by 800 pixels, naming it and setting it to windowed
	GLFWwindow* window = glfwCreateWindow(800, 800, "NoideGen", NULL, NULL);
	// Check if the window has been created otherwise exit the program
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Intorduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	// In this case the viewport goes form x = 0, y = 0, x = 800, y = 800
	glViewport(0, 0, 800, 800);

	//Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generate Vertex Buffer object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate Element Buffer object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidental clashing
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture

	Texture grass("886ce189807922dec4627e6fcbf9230df144f3c9.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	grass.texUnit(shaderProgram, "tex0", 0);


	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which shader program we want to use
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		grass.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primative
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the object we created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	grass.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}