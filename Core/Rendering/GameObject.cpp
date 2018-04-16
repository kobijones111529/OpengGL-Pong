#include "GameObject.h"

GameObject::GameObject() { }

GameObject::~GameObject() {
	for(GameObject* child : m_Children) {
		child->m_Parent = NULL;
	}
}

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

void GameObject::RemoveChild(GameObject* _child) {
	std::vector<GameObject*>::iterator it = std::find(m_Children.begin(), m_Children.end(), _child);
	if(it != m_Children.end()) {
		m_Children.erase(it);
		(*it)->m_Parent = NULL;
	}
}