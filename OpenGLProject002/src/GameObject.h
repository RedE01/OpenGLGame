#pragma once
#include <iostream>
#include "Model.h"

class GameObject {
private:
	unsigned int m_vbo;
	unsigned int m_ibo;
public:
	Model* m_model;
	unsigned int m_vao;
public: 
	GameObject(Model* modelRef);
	~GameObject();

	void draw();
private:
	void generateVAO(unsigned int& va);
	void generateVBO(unsigned int& vb, modelData* modelData);
	void genertateIBO(unsigned int& ib, modelData* modelData);
};