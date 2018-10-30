#pragma once
#include <vector>
#include <iostream>
#include "Texture.h"

struct modelData {
	std::vector<float> vert;
	unsigned int verticiesCount;
	unsigned int* indicies;
	unsigned int indiciesCount;
};

class Model {
private:
	Texture* m_modelTexture;
public:
	modelData m_modelData;
public:
	Model(const char* modelPath, Texture* modelTexture);
	~Model();

	Texture* getTexture();

private:
	modelData modelLoader(const char* modelPath);
};