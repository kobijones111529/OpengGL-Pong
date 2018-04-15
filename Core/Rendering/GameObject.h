#pragma once

#include <vector>

#include "../Transform.h"
#include "MeshRenderer.h"

class GameObject {
private:
	GameObject* m_Parent;
	std::vector<GameObject*> m_Children;
public:
	MeshRenderer* meshRenderer;
	Transform* transform;
	
	GameObject();
	~GameObject();
	
	virtual void Update();
	virtual void Render();
	
	void AddChild(GameObject* _child);
};