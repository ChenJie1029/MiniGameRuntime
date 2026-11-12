#pragma once

#include <string>
#include <glad/gl.h>

class Texture2D {
public:
	bool LoadFromFile(const std::string& path);
	GLuint GetId() const;
	void Destroy();

private:
	GLuint m_textureId = 0;
};