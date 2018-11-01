#pragma once
#include "GameObject.h"
#include <glm\glm.hpp>

class Terrain : public GameObject{
private:
	unsigned int m_size;
	unsigned int m_verticiesCount;
	glm::vec3 m_verticies[10000];
	unsigned int m_indicies[58806];
public:
	Terrain(unsigned int size, unsigned int verticiesCount);
	~Terrain();

};