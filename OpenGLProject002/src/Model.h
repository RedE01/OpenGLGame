#pragma once
#include <vector>
#include <iostream>
#include "Texture.h"
#include <array>

struct modelData {
	std::vector<float> vert;
	unsigned int verticiesCount;
	unsigned int* indicies;
	unsigned int indiciesCount;
	std::array<std::array<float, 101>, 101> heightMap;
};

class Model {
private:
	Texture* m_modelTexture;
	bool m_isTerrain;
public:
	modelData m_modelData;
public:
	Model(const char* modelPath, Texture* modelTexture, bool isTerrain);
	Model(unsigned int size, Texture* modelTexture);
	~Model();

	Texture* getTexture();

private:
	modelData modelLoader(const char* modelPath);
};