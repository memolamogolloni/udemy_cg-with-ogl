#include "Window.h"

Window::Window() {
	width = 800;
	height = 600;
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
}

int Window::Initialize() {
	if (!glfwInit()) {
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Disallow backwards compatibility 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Bind window context for GLEW to use it
	glfwMakeContextCurrent(mainWindow);

	// Set experimental features on
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("GLEW initialisation failed! %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Create viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
}
