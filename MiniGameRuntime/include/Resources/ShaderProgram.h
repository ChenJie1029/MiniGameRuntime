#pragma once

#include <glad/gl.h>

class ShaderProgram {
public:
	ShaderProgram() = default;
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator = (const ShaderProgram&) = delete;

	bool Build(const char* vertexSource, const char* fragmentSource);
	GLuint GetId() const;

private:
	GLuint m_programId = 0;
};
