#pragma once

#include "../Core/Rendering/Quad.h"

class Paddle : public Quad {
private:
public:
	float speed = 2.0f;
	
	Paddle();
	Paddle(float _width, float _height, glm::vec4 _color);
	~Paddle();
};