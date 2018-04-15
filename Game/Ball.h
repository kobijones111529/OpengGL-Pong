#pragma once

#include "../Core/Rendering/Quad.h"

class Ball : public Quad {
public:
	enum class VerticalCollision { NONE, TOP, BOTTOM };
	enum class HorizontalCollision { NONE, LEFT, RIGHT };
private:
public:
	VerticalCollision lastVerticalCollision = VerticalCollision::NONE;
	HorizontalCollision lastHorizontalCollision = HorizontalCollision::NONE;
	
	glm::vec3 velocity;
	
	Ball();
	~Ball();
};