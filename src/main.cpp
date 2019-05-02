#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;
const float PI = 3.14159265f;
const float DEG2RAD = PI / 180.0f;

GLuint VAO;
GLuint VBO;
GLuint IBO;
GLuint shader;
GLuint uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float currAngle = 0.0f;
float currAngleIncrement = 0.2;
bool sizeDirection = true;
float sizeIncrement = 0.005f;
float currSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex shader
static const char *vShader = "\
#version 330																												\n\
																																		\n\
layout (location = 0) in vec3 pos;																	\n\
out vec4 vCol;																											\n\
																																		\n\
uniform mat4 model;																									\n\
																																		\n\
void main() {																												\n\
	gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);							\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0);													\n\
}";

// Fragment shader
static const char *fShader = "\
#version 330																												\n\
																																		\n\
in vec4 vCol;																												\n\
out vec4 colour;																										\n\
																																		\n\
void main() {																												\n\
	colour = vCol;																										\n\
}";

void addShader(GLuint programId, const char* shaderSource, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	const GLchar *shaderSrc[1];
	shaderSrc[0] = shaderSource;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderSource);

	// Set shader source. Arguments:
	//	* Shader id
	//	* Files count
	//	* Shader source code
	//	* Length of the source code
	glShaderSource(shader, 1, shaderSrc, codeLength);
	// Compile shader
	glCompileShader(shader);

		// Reserve variables to save state and errors log from shaders compiling
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Link program to the GPU and check linker errors
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	// If failed when compiling, print logging data
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	// Attach shader
	glAttachShader(programId, shader);
}

// Compile shaders and check for errors
void compileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		printf("Error creating the shader program!\n");
		return;
	}

	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	// Reserve variables to save state and error logs from linking and validating shaders
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Link program to the GPU and check linker errors
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	// If failed when compiling, print logging data
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	// Validate program and check validation errors
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	// If validation failed, print logging data
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	// Get id of the uniform from vertex shader
	uniformModel = glGetUniformLocation(shader, "model");
};


void createTriangle() {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
		// Generate, bind and indices buffer data
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Generate and bind, VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Set buffer data. Arguments:
		//	* purpose of the buffer data. (GL_ARRAY_BUFFER -> vertex attributes)
		//	* size of the data
		//	* data
		//	* usage (STATIC -> not modified frequently)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Link buffer to vertex attributes at position 0. Arguments:
		//	* location of attribute in layout
		//	* size of group of values
		//	* type of the values
		//	* normalize values?
		//	* stride (used when passing many group of values in same buffer)
		//	* stating point of the group (also used to pass many groups to buffer)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// Enable vertex attributes at position 0
		glEnableVertexAttribArray(0);
		// Unbind VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Unbind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// 
	// Unbind VAO
	glBindVertexArray(0);

};

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
	if (!mainWindow) {
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

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Create viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	/* MAIN APP LOOP */
	createTriangle();
	compileShaders();

	while (!glfwWindowShouldClose(mainWindow)) {
		// Get user input
		glfwPollEvents();

		// Update position value
		if (direction) {
			triOffset += triIncrement;
		} else {
			triOffset -= triIncrement;
		}
		if (std::abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		// Update rotation value
		currAngle += currAngleIncrement;
		if (currAngle >= 360.0f) currAngle -= 360.0f;

		// Update scale value 
		if (sizeDirection) {
			currSize += sizeIncrement;
		} else {
			currSize -= sizeIncrement;
		}
		if (currSize > maxSize || currSize < minSize) {
			sizeDirection = !sizeDirection;
		}
		
		// RGBA clear color
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Enable the shader
		glUseProgram(shader);
			// Create a 4x4 matrix initialized to an identity matrix
			glm::mat4 model;
			// Multiply model by translation matrix
			// model = translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
			// Multiply model by rotation matrix (as it's in screen coordinates by now,
			// it will seem distorted)
			model = rotate(model, currAngle * DEG2RAD, glm::vec3(0.0f, 1.0f, 0.0f));
			// Multiply the model by the scale matrix (same as rotate)
			model = scale(model, glm::vec3(0.4f));

			// Send matrix pointer to shader as uniform. Arguments:
			//	* Uniform id
			//	* Count of matrices sent through the pointer
			//	* Transpose or not
			//	* Pointer to the 4x4 matrix
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			// Bind VAO
			glBindVertexArray(VAO);
				// Draw array in VAO. Arguments:
				//	* Drawing mode
				//	* Offset
				//	* Size of the array
				// Bind IBO
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				// Draw IBO
				// * Drawing mode
				// * Array size
				// * IBO type of variable
				// * Pointer to the indices (0 indicates none)
				glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
				// Unbind IBO
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			// Unbint VAO
			glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
