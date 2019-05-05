#pragma once

#include "stdio.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();
	~Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLint getBufferWidth();
	GLint getBufferHeight();

	bool getShouldClose();
	bool* getKeys();
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers();

private:
	static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow *window, double xPos, double yPos);
	void createCallbacks();

	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
};