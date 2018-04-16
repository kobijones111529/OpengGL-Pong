#pragma once

#include "../Transform.h"

class Component {
private:
public:
	Component();
	~Component();
	
	virtual void Update(Transform _transform) = 0;
	virtual void Render(Transform _transform) = 0;
};