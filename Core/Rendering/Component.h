#pragma once

#include "../Transform.h"

class Component {
private:
public:
	enum ComponentType : unsigned int {
		MESH_RENDERER = 1
	};
	
	Component();
	virtual ~Component();
	
	virtual void Update(Transform _transform) = 0;
	virtual void Render(Transform _transform) = 0;
};