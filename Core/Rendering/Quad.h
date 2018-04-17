#pragma once

#include "GameObject.h"

class Quad : public GameObject {
public:
	float width;
	float height;
	glm::vec4 color;
	
	Quad();
	Quad(float _width, float _height, glm::vec4 _color);
	~Quad();
	
	void UpdateMesh();
};