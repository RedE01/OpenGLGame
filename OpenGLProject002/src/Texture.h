#pragma once
#include <string>

class Texture {
private:
	int m_width = 0;
	int m_height = 0;
	int m_bpp = 0;
	unsigned char* m_data;
	unsigned int m_textureID;
	float m_tiles;
public:
	Texture(const char* imgPath, float tiles);
	~Texture();

	float getTiles();

	void use();

};