#include "GameObject.h"

GameObject::GameObject() { }
GameObject::~GameObject() { }

void GameObject::Update() {
	for(GameObject* child : m_Children) {
		child->Update();
	}
	
	if(meshRenderer) {
		meshRenderer->Update(transform);
	}
}

void GameObject::Render() {
	for(GameObject* child : m_Children) {
		child->Render();
	}
	
	if(meshRenderer) {
		meshRenderer->Render(transform);
	}
}

void GameObject::AddChild(GameObject* _child) {
	m_Children.push_back(_child);
	_child->m_Parent = this;
}