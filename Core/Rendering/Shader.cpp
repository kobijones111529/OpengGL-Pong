#include "Shader.h"

Shader::Shader() { }
Shader::~Shader() { }

void Shader::Use() {
	glUseProgram(m_ID);
}

void Shader::Unuse() {
	glUseProgram(NULL);
}

bool Shader::Init(const std::string& _vertexShader, const std::string& _fragmentShader) {
	std::string vertexShaderSrcStr = ReadFile(_vertexShader);
	std::string fragmentShaderSrcStr = ReadFile(_fragmentShader);
	const char* vertexShaderSrc = vertexShaderSrcStr.c_str();
	const char* fragmentShaderSrc = fragmentShaderSrcStr.c_str();
	
	bool success = true;
	int status;
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE) {
		int length;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		char log[1024];
		glGetShaderInfoLog(vertexShader, 1024, &length, log);
		std::cout << length << std::endl;
		std::cerr << "Shader Error: Failed to compile \"" << _vertexShader << "\": " << log << std::endl;
		success = false;
	}
	
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE) {
		int length;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		char log[1024];
		glGetShaderInfoLog(fragmentShader, 1024, &length, log);
		std::cerr << "Shader Error: Failed to compile \"" << _fragmentShader << "\": " << log << std::endl;
		success = false;
	}
	
	if(!success) return false;
	
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status != GL_TRUE) {
		int length;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
		char log[1024];
		glGetProgramInfoLog(program, 1024, &length, log);
		std::cerr << "Shader Error: Failed to link \"" << _vertexShader << "\" and \"" << _fragmentShader << "\": " << log;
		success = false;
	}
	
	if(!success) return false;
	
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	m_ID = program;
	
	return success;
}

int Shader::GetUniformLocation(const std::string& _name) {
	int location = glGetUniformLocation(m_ID, _name.c_str());
	if(location == -1) {
		std::cout << "Shader Warning: Uniform \"" << _name << "\" not found!" << std::endl;
	}
	return location;
}

void Shader::SetUniform4f(const std::string& _name, float _v0, float _v1, float _v2, float _v3) {
	glUseProgram(m_ID);
	glUniform4f(GetUniformLocation(_name), _v0, _v1, _v2, _v3);
}

void Shader::SetUniformMat4fv(const std::string& _name, const float* _value) {
	glUseProgram(m_ID);
	glUniformMatrix4fv(GetUniformLocation(_name), 1, GL_FALSE, _value);
}