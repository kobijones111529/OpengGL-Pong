#include "GameObject.h"

template<>
void GameObject::AddComponent<MeshRenderer>();

template<>
void GameObject::RemoveComponent<MeshRenderer>();

template<>
MeshRenderer* GameObject::GetComponent<MeshRenderer>();

GameObject::GameObject() {
	MeshRenderer* mr = new MeshRenderer;
	m_Components.insert({ (unsigned int)mr->type, mr });
}

GameObject::~GameObject() {
	for(GameObject* child : m_Children) {
		child->m_Parent = NULL;
	}
	
	while(m_Components.end() != m_Components.begin()) {
		delete m_Components.begin()->second;
		m_Components.erase(m_Components.begin());
	}
}

void GameObject::Update() {
	for(GameObject* child : m_Children) {
		child->Update();
	}
	
	for(std::pair<unsigned int, Component*> component : m_Components) {
		component.second->Update(transform);
	}
}

void GameObject::Render() {
	for(GameObject* child : m_Children) {
		child->Render();
	}
	
	for(std::pair<unsigned int, Component*> component : m_Components) {
		component.second->Render(transform);
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

template<typename T>
void GameObject::AddComponent() {
	std::cerr << "Invalid type!" << std::endl;
}

template<>
void GameObject::AddComponent<MeshRenderer>() {
	MeshRenderer* mr = new MeshRenderer;
	m_Components.insert({ Component::ComponentType::MESH_RENDERER, mr });
}

template<typename T>
void GameObject::RemoveComponent() {
	std::cerr << "Invalid type!" << std::endl;
}

template<>
void GameObject::RemoveComponent<MeshRenderer>() {
	std::unordered_map<unsigned int, Component*>::iterator it = m_Components.find(Component::ComponentType::MESH_RENDERER);
	if(it != m_Components.end()) {
		delete it->second;
		m_Components.erase(it);
	} else {
		std::cerr << "Component type not associated with GameObject!" << std::endl;
	}
}

template<typename T>
T* GameObject::GetComponent() {
	std::cerr << "Invalid type!" << std::endl;
	return NULL;
}

template<>
MeshRenderer* GameObject::GetComponent<MeshRenderer>() {
	std::unordered_map<unsigned int, Component*>::iterator it = m_Components.find(Component::ComponentType::MESH_RENDERER);
	if(it != m_Components.end()) {
		return (MeshRenderer*)it->second;
	} else {
		std::cerr << "Component type not associated with GameObject!" << std::endl;
		return NULL;
	}
}