#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

// GENERATE, BIND AND SET VAO, VBO and IBO
void Mesh::CreateMesh(glm::vec3 *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

  // Generate VAO
	glGenVertexArrays(1, &VAO);
	// Bind VAO
	glBindVertexArray(VAO);

		// Generate, bind and set IBO
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

		// Generate bind and set VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

		// Link to vertex attributes at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// Unbind VBO and IBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

// DRAW 
void Mesh::RenderMesh() {
	// Bind VAO
	glBindVertexArray(VAO);
		// Bind, draw and unbind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Unbind VAO
	glBindVertexArray(0);
}

// CLEAR ALL BUFFERS
void Mesh::ClearMesh() {
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}


Mesh::~Mesh() {
	ClearMesh();
}
