#pragma once
#include <iostream>
#include <memory>
#include <vector>

struct modelData {
	std::vector<float> vert;
	unsigned int verticiesCount;
	unsigned int* indicies;
	unsigned int indiciesCount;
};

class GameObject {
public:
	modelData m_modelData;
public: 
	GameObject(const char* modelPath);
	~GameObject();

	modelData* getModelData();
private:
	modelData generateModelData(const char* modelPath);
};