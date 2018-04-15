#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

static std::string ReadFile(const std::string& _file) {
	std::ifstream file("Core/Rendering/Shaders/" + _file);
	if(!file.is_open()) {
		std::cerr << "File IO Error: Failed to open \"" << _file << "\"" << std::endl;
		return NULL;
	}
	
	std::stringstream ss;
	std::string line;
	while(getline(file, line)) {
		ss << line << '\n';
	}
	
	return ss.str();
}