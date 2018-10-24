#include "GameObject.h"
#include <GL\glew.h>
#include "Debug.h"


GameObject::GameObject(Model* modelRef) 
	: m_model(modelRef) {

	generateVAO(m_vao);
	generateVBO(m_vbo, &m_model->m_modelData);
	genertateIBO(m_ibo, &m_model->m_modelData);
}

GameObject::~GameObject() {
}

void GameObject::draw() {
	GLCall(glDrawElements(GL_TRIANGLES, m_model->m_modelData.indiciesCount, GL_UNSIGNED_INT, nullptr));
}

void GameObject::generateVAO(unsigned int & va) {
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);
}

void GameObject::generateVBO(unsigned int & vb, modelData* modelData) {
	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, modelData->verticiesCount * sizeof(float), &modelData->vert[0], GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))));
}

void GameObject::genertateIBO(unsigned int & ib, modelData* modelData) {
	GLCall(glGenBuffers(1, &ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelData->indiciesCount * sizeof(float), modelData->indicies, GL_DYNAMIC_DRAW));
}
