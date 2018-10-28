#pragma once
#include <iostream>
#include "Model.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class GameObject {
private:
	unsigned int m_vbo;
	unsigned int m_ibo;
public:
	Model* m_model;
	unsigned int m_vao;

	glm::mat4 m_translation;
	glm::mat4 m_rotationX;
	glm::mat4 m_rotationY;
	glm::mat4 m_rotationZ;
public: 
	GameObject(Model* modelRef);
	~GameObject();

	void draw();
private:
	void generateVAO(unsigned int& va);
	void generateVBO(unsigned int& vb, modelData* modelData);
	void genertateIBO(unsigned int& ib, modelData* modelData);
};