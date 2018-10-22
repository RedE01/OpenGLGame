#include "Shader.h"
#include "Debug.h"
#include <GL\glew.h>
#include <fstream>
#include <sstream>

Shader::Shader(std::string & shaderPath) {
	m_programID = glCreateProgram();

	shaderSources sources = createShaderSources(shaderPath);

	unsigned int vs = CreateShader(sources.vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CreateShader(sources.fragmentShader, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(m_programID, vs));
	GLCall(glAttachShader(m_programID, fs));
	GLCall(glLinkProgram(m_programID));
	GLCall(glValidateProgram(m_programID));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	GLCall(glUseProgram(m_programID));
}

Shader::~Shader() {
}

shaderSources Shader::createShaderSources(const std::string& shaderPath) {
	std::ifstream stream(shaderPath);

	std::string line;
	std::stringstream ss[2];

	enum class shaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	shaderType type = shaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = shaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = shaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	stream.close();
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(std::string& shaderSource, int type) {
	const char* src = shaderSource.c_str();

	GLCall(unsigned int shader = glCreateShader(type));
	GLCall(glShaderSource(shader, 1, &src, nullptr));
	GLCall(glCompileShader(shader));

	int result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(shader, length, &length, message));
		std::cout << "Failed to compile shader: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(shader));
	}
	return shader;
}