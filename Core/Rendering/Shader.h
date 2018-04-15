#pragma once

#include <GL/glew.h>

#include "../File.h"

class Shader {
private:
	unsigned int m_ID;
	
	int GetUniformLocation(const std::string& _name);
public:
	Shader();
	~Shader();
	
	void Use();
	void Unuse();
	
	bool Init(const std::string& _vertexShader, const std::string& _fragmentShader);
	
	void SetUniform4f(const std::string& _name, float _v0, float _v1, float _v2, float _v3);
	void SetUniformMat4fv(const std::string& _name, const float* _value);
};