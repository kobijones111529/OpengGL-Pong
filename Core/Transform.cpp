#include "Transform.h"

Transform::Transform() { }
Transform::~Transform() { }

glm::mat4 Transform::GetTransformation() const {
	glm::mat4 transformation;
	transformation = glm::translate(transformation, translation);
	return transformation;
}

void Transform::Translate(glm::vec3 _translation) {
	translation += _translation;
}