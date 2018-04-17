#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>

#include "../Transform.h"
#include "MeshRenderer.h"
#include "Component.h"

class GameObject {
private:
	GameObject* m_Parent;
	std::vector<GameObject*> m_Children;
	std::unordered_map<unsigned int, Component*> m_Components;
public:
	MeshRenderer* meshRenderer;
	Transform transform;
	
	GameObject();
	~GameObject();
	
	virtual void Update();
	virtual void Render();
	
	void AddChild(GameObject* _child);
	void RemoveChild(GameObject* _child);

	template<typename T>
	void AddComponent();
	
	template<typename T>
	void RemoveComponent();
	
	template<typename T>
	T* GetComponent();
};