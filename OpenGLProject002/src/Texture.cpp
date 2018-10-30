#include "Texture.h"
#include "stb_image\stb_image.h"
#include "GL\glew.h"
#include "Debug.h"

Texture::Texture(const char* imgPath, float tiles) 
		: m_tiles(tiles) {
	stbi_set_flip_vertically_on_load(true);
	m_data = stbi_load(imgPath, &m_width, &m_height, &m_bpp, 4);

	GLCall(glGenTextures(1, &m_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (m_data) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << "FAILED TO READ IMAGE" << std::endl;
	}
	stbi_image_free(m_data);
}

Texture::~Texture() {
}

float Texture::getTiles() {
	return m_tiles;
}

void Texture::use() {
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}
