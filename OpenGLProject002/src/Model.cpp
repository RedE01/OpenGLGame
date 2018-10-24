#include "Model.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

Model::Model(const char* modelPath) {
	m_modelData = modelLoader(modelPath);
}

Model::~Model() {
}

modelData Model::modelLoader(const char* modelPath) {
	std::fstream stream;
	stream.open(modelPath);
	std::stringstream ss;

	ss << stream.rdbuf();
	stream.close();

	std::string temp = ss.str();
	const char* fileDataString = temp.c_str();

	float number = 0.0f;
	float decimalMult = -1.0f;
	float sign = 1.0f;

	std::vector<float> vert;
	std::vector<float> textures;
	std::vector<float> normals;

	std::vector<float> verticies;

	enum NumType {
		NONE = -1, V = 0, VT = 1, VN = 2, F = 3
	};
	NumType type = NumType::NONE;

	unsigned int numb = 0;
	unsigned int len = strlen(fileDataString);
	for (unsigned int i = 0; i < len; i++) {
		if (fileDataString[i] >= '0' && fileDataString[i] <= '9') {
			if (decimalMult == -1) {
				number = number * 10.0f + fileDataString[i] - '0';
			}
			else {
				number = number + (fileDataString[i] - '0') * decimalMult;
				decimalMult *= 0.1f;
			}
		}
		else if (fileDataString[i] == '.') {
			decimalMult = 0.1f;
		}
		else if (fileDataString[i] == '-') {
			sign = -1.0f;
		}
		else if (fileDataString[i] == ' ' || fileDataString[i] == '\n' || fileDataString[i] == '/') {
			number = number * sign;

			if (type != NumType::NONE) {
				switch (type) {
				case NumType::V:
					vert.push_back(number);
					break;
				case NumType::VT:
					textures.push_back(number);
					break;
				case NumType::VN:
					normals.push_back(number);
					break;
				case NumType::F:
					for (unsigned int j = 0; j < 3; j++) {
						unsigned int index = (number - 1) * 3 + j;
						switch (numb) {
						case 0:
							verticies.push_back(vert[index]);
							break;
						case 1:
							verticies.push_back(textures[index]);
							break;
						case 2:
							verticies.push_back(normals[index]);
							break;
						}
					}
					numb++;
					if (numb > 2) numb = 0;
				}
			}

			decimalMult = -1.0f;
			number = 0.0f;
			sign = 1.0f;

			if (fileDataString[i] == '\n') {
				if (fileDataString[i + 1] == 'v') {
					int offset = 2;
					if (fileDataString[i + 2] == 't') {
						type = NumType::VT;
						offset = 3;
					}
					else if (fileDataString[i + 2] == 'n') {
						type = NumType::VN;
						offset = 3;
					}
					else {
						type = NumType::V;
					}
					i += offset;
				}
				else if (fileDataString[i + 1] == 'f') {
					type = NumType::F;
					i += 2;
				}
				else {
					type = NumType::NONE;
				}
			}
		}
	}

	unsigned int vertCount = verticies.size();
	unsigned int indexCount = vertCount / 9;

	unsigned int* indicies = new unsigned int[indexCount];
	for (unsigned int i = 0; i < indexCount; i++) {
		indicies[i] = i;
	}
	return { verticies, vertCount, indicies, indexCount };
}
