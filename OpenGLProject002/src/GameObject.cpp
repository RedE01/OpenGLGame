#include "GameObject.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

GameObject::GameObject(const char * modelPath) {
	m_modelData = generateModelData(modelPath);
}

GameObject::~GameObject() {
}

modelData * GameObject::getModelData() {
	return &m_modelData;
}

modelData GameObject::generateModelData(const char * modelPath) {
	std::vector<std::array<float, 3>> positions;
	std::vector<std::array<float, 3>> textures;
	std::vector<std::array<float, 3>> normals;
	std::vector<float> vert;
	unsigned int indiciesCount = 0;

	std::ifstream test;
	test.open(modelPath);

	enum DataType {
		NONE = -1, V = 0, VT = 1, VN = 2, F = 3
	};
	DataType type = DataType::NONE;
	int startPos = 0;
	std::string line;
	if (test.is_open()) {
		while (getline(test, line)) {
			if (line.substr(0, 2) == "v ") {
				type = DataType::V;
				startPos = 2;
			}
			else if (line.substr(0, 3) == "vt ") {
				type = DataType::VT;
				startPos = 3;
			}
			else if (line.substr(0, 3) == "vn ") {
				type = DataType::VN;
				startPos = 3;
			}
			else {
				type = DataType::NONE;
			}

			if (line.substr(0, 2) == "f ") {
				std::stringstream linestream(line.substr(2));
				std::string segment;
				while (std::getline(linestream, segment, ' ')) {
					std::stringstream segmentstream(segment);
					std::string segpart;
					indiciesCount++;
					for (int i = 0; std::getline(segmentstream, segpart, '/'); i++) {
						int number = std::stoi(segpart);
						switch (i) {
						case 0:
							for (int j = 0; j < 3; j++) {
								vert.push_back(positions[number - 1][j]);
							}
							break;
						case 1:
							for (int j = 0; j < 3; j++) {
								vert.push_back(textures[number - 1][j]);
							}
							break;
						case 2:
							for (int j = 0; j < 3; j++) {
								vert.push_back(normals[number - 1][j]);
							}
							break;
						}
					}
				}
			}

			if (type != DataType::NONE) {
				std::istringstream v(line.substr(startPos));

				std::array<float, 3> temp;
				for (int i = 0; i < 3; i++) {
					v >> temp[i];
				}
				switch (type) {
				case DataType::V:
					positions.push_back(temp);
					break;
				case DataType::VT:
					textures.push_back(temp);
					break;
				case DataType::VN:
					normals.push_back(temp);
					break;
				}
			}
		}
		test.close();
	}

	float* verticies = &vert[0];
	unsigned int verticiesCount = vert.size();

	unsigned int* indicies = new unsigned int[indiciesCount];

	for (int i = 0; i < indiciesCount; i++) {
		indicies[i] = i;
	}
	return { vert, verticiesCount, indicies, indiciesCount };
}
