#pragma once

#include "Component.h"
#include "../Transform.h"
#include "Mesh.h"
#include "Shader.h"

class MeshRenderer : public Component {
private:
public:
	Mesh* mesh;
	Shader* shader;
	
	MeshRenderer();
	~MeshRenderer();
	
	void Update(const Transform* _transform);
	void Render(const Transform* _transform);
};