#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh {
public:
	Mesh();
	~Mesh();

	void CreateMesh(glm::vec3 *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();


private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

