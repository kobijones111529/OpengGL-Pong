#include "Mesh.h"

Mesh::Mesh() {
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void Mesh::SetVertices(const Vertex* _vertices, const unsigned int* _indices, unsigned int _vertexCount, unsigned int _indexCount) {
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(Vertex), _vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(unsigned int), _indices, GL_STATIC_DRAW);
	
	m_VertexCount = _vertexCount;
	m_IndexCount = _indexCount;
}

void Mesh::Draw() {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (const void*)0);
	
	glDeleteVertexArrays(1, &vao);
}