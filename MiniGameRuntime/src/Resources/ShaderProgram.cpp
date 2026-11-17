#include <iostream>

#include "Resources/ShaderProgram.h"

namespace {
    GLuint CompileShader(GLenum type, const char* source, const char* label) {
        const GLuint shader = glCreateShader(type);
        if (shader == 0) {
            std::cerr << "Failed to create " << label << " shader.\n";
            return 0;
        }

        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_TRUE) {
            return shader;
        }

        char log[1024]{};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << label << " shader compilation failed:\n" << log << '\n';
        glDeleteShader(shader);
        return 0;
    }
}

ShaderProgram::~ShaderProgram() {
    if (m_programId != 0) {
        glDeleteProgram(m_programId);
    }
}

bool ShaderProgram::Build(const char* vertexSource, const char* fragmentSource) {
    if (m_programId != 0 || vertexSource == nullptr || fragmentSource == nullptr) {
        return false;
    }

    const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource, "Vertex");
    const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource, "Fragment");
    if (vertexShader == 0 || fragmentShader == 0) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    const GLuint program = glCreateProgram();
    if (program == 0) {
        std::cerr << "Failed to create shader program.\n";
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        char log[1024]{};
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cerr << "Shader program linking failed:\n" << log << '\n';
        glDeleteProgram(program);
        return false;
    }

    m_programId = program;
    return true;
}

GLuint ShaderProgram::GetId() const {
    return m_programId;
}