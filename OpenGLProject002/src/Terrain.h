#pragma once
#include "Model.h"
#include "Texture.h"
#include <glm\glm.hpp>

class Terrain : public Model {
private:
	unsigned int m_size;
	float* m_heightMap;
public:
	Terrain(unsigned int size, unsigned int verticiesCount, Texture* texture);
	~Terrain();

	float getHeight(int x, int z);
private:
	modelData generateTerrainModelData(int size);
};