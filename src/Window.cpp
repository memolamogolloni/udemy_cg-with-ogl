#include "Window.h"

Window::Window() {
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
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

	// Handle key and mouse input
	createCallbacks();
	// Hide mouse cursor
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// Set events detection
	glfwSetWindowUserPointer(mainWindow, this);
}

GLint Window::getBufferWidth() {
	return bufferWidth;
}
GLint Window::getBufferHeight() {
	return bufferHeight;
}

bool Window::getShouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

bool* Window::getKeys() {
	return keys;
}

GLfloat Window::getXChange() {
	GLfloat change = xChange;
	xChange = 0;
	return change;
}

GLfloat Window::getYChange() {
	GLfloat change = yChange;
	yChange = 0;
	return change;
}

void Window::swapBuffers() {
	glfwSwapBuffers(mainWindow);
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
	Window *win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// Check for exit keys
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	// Sanitize keys
	if (key > 0 && key < 1024) {
		// Check key pressed
		if (action == GLFW_PRESS) {
			win->keys[key] = true;
			printf("Pressed: %d\n", key);
			// Check key released
		} else if (action == GLFW_RELEASE) {
			win->keys[key] = false;
			printf("Released: %d\n", key);
		}
	}
}

void Window::handleMouse(GLFWwindow *window, double xPos, double yPos) {
	Window *win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (win->mouseFirstMoved) {
		win->lastX = xPos;
		win->lastY = yPos;
		win->mouseFirstMoved = false;
	}

	win->xChange = xPos - win->lastX;
	win->yChange = yPos - win->lastY;

	win->lastX = xPos;
	win->lastY = yPos;
}

void Window::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

