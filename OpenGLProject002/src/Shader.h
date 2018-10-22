#pragma once
#include <iostream>
#include <string>

struct shaderSources {
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader {
private:
	unsigned int m_programID;
public:
	Shader(std::string& shaderPath);
	~Shader();

	shaderSources createShaderSources(const std::string& shaderPath);
	unsigned int CreateShader(std::string& shaderSource, int type);

	inline unsigned int getProgramID() { return m_programID; };
};