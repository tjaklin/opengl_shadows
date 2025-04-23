#pragma once

#include "GL/glew.h"
#include <fstream>

enum ShaderType { NONE = 0, VERTEX = GL_VERTEX_SHADER, GEOMETRY = GL_GEOMETRY_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

class Shader
{
public:
    Shader(const char* vertex_filepath, const char* fragment_filepath);
    Shader(const char* vertex_filepath, const char* geometry_filepath, const char* fragment_filepath);
    ~Shader();

    void Bind() const;
    GLuint Get() const;

private:
    GLuint ParseAndCompile(ShaderType type, std::ifstream& source_file);
    bool Link(GLuint vertex, GLuint geometry, GLuint fragment);

private:
    GLuint _program;
};