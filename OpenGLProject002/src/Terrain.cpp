#include "Terrain.h"
#include "PerlinNoise.h"

Terrain::Terrain(unsigned int size, unsigned int verticiesCount, Texture* texture)
		: m_size(size), Model(generateTerrainModelData(size), texture) {
}

Terrain::~Terrain() {
	delete[] m_heightMap;
}

float Terrain::getHeight(int x, int z) {
	return m_heightMap[x + z * (m_size + 1)];
}

modelData Terrain::generateTerrainModelData(int size) {
	int halfSize = size / 2;
	int vertRow = size + 1;

	m_heightMap = new float[vertRow * vertRow];

	std::vector<float> vertData;
	unsigned int vertCount = vertRow * vertRow * 6;
	vertData.reserve(vertCount);
	unsigned int indCount = size * size * 6;
	unsigned int* indData = new unsigned int[indCount];

	unsigned int indIterator = 0;
	for (int z = 0; z < vertRow; z++) {
		for (int x = 0; x < vertRow; x++) {
			float height = Perlin::ValueNoise_2D(x, z) * 100.0;
			m_heightMap[x + z * vertRow] = height;

			//positions
			vertData.push_back(x - halfSize);
			vertData.push_back(height);
			vertData.push_back(z - halfSize);

			//normals
			vertData.push_back(0.0f);
			vertData.push_back(0.0f);
			vertData.push_back(0.0f);
			
			if (x < size && z < size) {
				indData[indIterator + 0] = (x + 1) + (z + 1) * vertRow;
				indData[indIterator + 1] = (x + 1) + (z + 0) * vertRow;
				indData[indIterator + 2] = (x + 0) + (z + 0) * vertRow;

				indData[indIterator + 3] = (x + 0) + (z + 0) * vertRow;
				indData[indIterator + 4] = (x + 0) + (z + 1) * vertRow;
				indData[indIterator + 5] = (x + 1) + (z + 1) * vertRow;

				indIterator += 6;
			}
		}
	}

	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {
			unsigned int xPos = (x + z * vertRow) * 6;
			glm::vec3 pos1(vertData[xPos], vertData[xPos + 1], vertData[xPos + 2]);
			unsigned int xPos2 = ((x + 1) + (z + 1) * vertRow) * 6;
			glm::vec3 pos2(vertData[xPos2], vertData[xPos2 + 1], vertData[xPos2 + 2]);
			unsigned int xPos3 = (x + (z + 1) * vertRow) * 6;
			glm::vec3 pos3(vertData[xPos3], vertData[xPos3 + 1], vertData[xPos3 + 2]);

			glm::vec3 vec1(pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z);
			glm::vec3 vec2(pos3.x - pos1.x, pos3.y - pos1.y, pos3.z - pos1.z);
			glm::vec3 cross = -glm::cross(vec1, vec2);
			vertData[xPos + 3] = cross.x;
			vertData[xPos + 4] = cross.y;
			vertData[xPos + 5] = cross.z;
		}
	}

	return { vertData, vertCount, indData, indCount };
}
