#include "Terrain.h"

Terrain::Terrain(unsigned int size, unsigned int verticiesCount)
		: GameObject(0.0f, 0.0f, 0.0f), m_size(size), m_verticiesCount(verticiesCount) {

}

Terrain::~Terrain() {
}

/*for (int i = 0; i < 10000; i++) {
float x = i % 100;
float z = std::floor(i / 100);
m_verticies[i] = glm::vec3(x, 0, z);
}

unsigned int indCount = 58806;
for (int i = 0; i < indCount; i+=3) {
int a = i / 3;
if (i < indCount / 2) {
m_indicies[i] = a;
m_indicies[i + 1] = a + 100;
m_indicies[i + 2] = a + 101;
}
else {
m_indicies[i] = a + 101;
m_indicies[i + 1] = a + 1;
m_indicies[i + 2] = a;
}
}*/
