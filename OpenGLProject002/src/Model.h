#pragma once
#include <vector>
#include <iostream>

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

	inline void printVertCount() {
		std::cout << m_modelData.verticiesCount << std::endl;
	}

private:
	modelData modelLoader(const char* modelPath);
};