#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"


// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

// Math constants
const float PI = 3.14159265f;
const float DEG2RAD = PI / 180.0f;

// Classes instances
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Vertex and fragment shaders location
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";


void CreateObjects()  {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	glm::vec3 vertices[] = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 4, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 4, 12);
	meshList.push_back(obj2);
}

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;

	// Create perspective projection matrix.
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {
		// Get user input
		glfwPollEvents();

		// RGBA clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

			// Create and apply transforms to the model matrix
			glm::mat4 model;	
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
			// Send matrix pointer to shader as uniform.
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			// Bind VAO, bind IBO, draw and unbind
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[0]->RenderMesh();

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
