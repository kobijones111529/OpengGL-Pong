#pragma once

#include "GameObject.h"

class Quad : public GameObject {
public:
	double width;
	double height;
	glm::vec4 color;
	
	Quad();
	~Quad();
	
	void UpdateMesh();
};