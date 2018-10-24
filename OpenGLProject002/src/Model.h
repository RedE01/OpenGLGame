#pragma once
#include <vector>

struct modelData {
	std::vector<float> vert;
	unsigned int verticiesCount;
	unsigned int* indicies;
	unsigned int indiciesCount;
};

class Model {
public:
	modelData m_modelData;
public:
	Model(const char* modelPath);
	~Model();

private:
	modelData modelLoader(const char* modelPath);
};