#pragma once

#include "glm/glm.h"

class Transform {
private:
public:
	glm::vec3 translation;
	
	Transform();
	~Transform();
	
	glm::mat4 GetTransformation() const;
	
	void Translate(glm::vec3 _translation);
};