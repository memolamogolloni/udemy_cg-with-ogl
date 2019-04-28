#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	if (!glfwInit()) {
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	/* WINDOW PROPERTIES */
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Disallow backwards compatibility 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window setting widthm height and title
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Bind window context for GLEW to use it
	glfwMakeContextCurrent(mainWindow);

	// Set experimental features on
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Create viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	/* Main app loop */
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get user input
		glfwPollEvents();

		// RGBA clear color
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}