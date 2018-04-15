#pragma once

#include "../Transform.h"

class Component {
private:
public:
	Component();
	~Component();
	
	virtual void Update(const Transform* _transform) = 0;
	virtual void Render(const Transform* _transform) = 0;
};