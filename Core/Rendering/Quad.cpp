#include "Quad.h"

Quad::Quad() { }
Quad::~Quad() { }

void Quad::UpdateMesh() {
	std::vector<Vertex> vertices {
		{ glm::vec3(0.0f , 0.0f   , 0.0f), color },
		{ glm::vec3(width, 0.0f   , 0.0f), color },
		{ glm::vec3(width, -height, 0.0f), color },
		{ glm::vec3(0.0f , -height, 0.0f), color }
	};
	
	std::vector<unsigned int> indices { 0, 1, 2, 2, 3, 0 };
	
	GetComponent<MeshRenderer>()->mesh.SetVertices(&vertices[0], &indices[0], 4, 6);
}