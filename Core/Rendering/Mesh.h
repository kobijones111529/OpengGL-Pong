#pragma once

#include <GL/glew.h>

#include "glm/glm.h"

struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
};

class Mesh {
private:
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_VertexCount = 0;
	unsigned int m_IndexCount = 0;
public:
	Mesh();
	~Mesh();
	
	void SetVertices(const Vertex* _vertices, const unsigned int* _indices, unsigned int _vertexCount, unsigned int _indexCount);
	void Draw();
};