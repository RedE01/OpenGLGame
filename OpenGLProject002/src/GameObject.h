#pragma once
#include <iostream>
#include "Model.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Shader.h"

class GameObject {
private:
	unsigned int m_vbo;
	unsigned int m_ibo;
public:
	Model* m_model;
	unsigned int m_vao;

	glm::vec3 m_pos;
	glm::mat4 m_translation;
	glm::mat4 m_rotationX;
	glm::mat4 m_rotationY;
	glm::mat4 m_rotationZ;
public: 
	GameObject(Model* modelRef, float x, float y, float z);
	~GameObject();

	void draw(Shader& shader);
	void setPos(float x, float y, float z);
	void move(float x, float y, float z);
private:
	void generateVAO(unsigned int& va);
	void generateVBO(unsigned int& vb, modelData* modelData);
	void genertateIBO(unsigned int& ib, modelData* modelData);
};