#define STB_IMAGE_IMPLEMENTATION

#include "ThirdParty/stb_image.h"
#include "Resources/Texture2D.h"

#include <iostream>

bool Texture2D::LoadFromFile(const std::string& path) {
	int width = 0;
	int height = 0;
	int channels = 0;

	unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (pixels == nullptr) {
		std::cerr << "Failed to load texture: " << path << '\n';
		std::cerr << "Reason: " << stbi_failure_reason() << '\n';
		return false;
	}

	glGenTextures(1, &m_textureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(pixels);

	return true;
}

GLuint Texture2D::GetId() const {
	return m_textureId;
}

void Texture2D::Destroy() {
	if (m_textureId == 0) {
		return;
	}

	glDeleteTextures(1, &m_textureId);
	m_textureId = 0;
}