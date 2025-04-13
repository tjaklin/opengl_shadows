#include "../inc/Shader.hpp"

#include <sstream>
#include <cstdio>
#include <cstring>

Shader::Shader(const char* vertex_filepath, const char* fragment_filepath)
    : Shader(vertex_filepath, nullptr, fragment_filepath)
{
    // Explicitly passing 'nullptr' to the overloaded constructor
    //  which takes in a geometry shader.
    // Check the overloaded constructor for an explanation.
}

Shader::Shader(const char* vertex_filepath, const char* geometry_filepath, const char* fragment_filepath)
    : _program(0)
{
    // Vertex
    std::ifstream vertex_file(vertex_filepath);
    GLuint vertex = ParseAndCompile(ShaderType::VERTEX, vertex_file);

    // Geometry. This type of shader is optional and will not be used
    //  in all shader programs. If value of 'geometry_filepath' is
    //  'nullptr', the intention of the method-caller is to not
    //  use the geometry shader. This means that that no warning messages
    //  will get shown for 'geometry_filepath' in that case.
    GLuint geometry = 255;
    if (geometry_filepath != nullptr)
    {
        std::ifstream geometry_file(geometry_filepath);
        geometry = ParseAndCompile(ShaderType::GEOMETRY, geometry_file);
    }

    // Fragment
    std::ifstream fragment_file(fragment_filepath);
    GLuint fragment = ParseAndCompile(ShaderType::FRAGMENT, fragment_file);

    // Link shaders into program
    if (!Link(vertex, geometry, fragment))
    {
        // If all went well, a valid shader program value is now present
        // in '_program' ! If not, return to prevent object construction (?)
        return;
    }
}

Shader::~Shader()
{
    glDeleteProgram(_program);
    _program = 0;
}

void Shader::Bind() const
{
    glUseProgram(_program);
}

GLuint Shader::Get() const
{
    return _program;
}

GLuint Shader::ParseAndCompile(ShaderType type, std::ifstream& source_file)
{
    // TODO: Probaj ovo sa 'istream_iterator' rjesit, ko u knjigi!
    if (!source_file.is_open())
    {
        return 255; // This is code for "Invalid Shader handle".
    }

    // Parse
    std::stringstream shader_stream;
    std::string source_line = "";
    while (getline(source_file, source_line, '\n'))
    {
        shader_stream << source_line << '\n';
    }

    auto source_str = shader_stream.str();
    auto source_c = source_str.c_str();
    auto source_length = strlen(source_c);

    // Compile
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source_c, (const GLint*) &source_length);
    glCompileShader(shader);

    // Check for errors
    int compile_status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        // Compilation error detected. Get more info.
        GLchar shader_info[1024];
        GLsizei info_length = 0;
        glGetShaderInfoLog(shader, sizeof(shader_info), &info_length, shader_info);
        printf("[Shader] Compilation error detected. Info:\n%s", shader_info);

        glDeleteShader(shader);
        return 255; // This is code for "Invalid Shader handle".
    }

    return shader;
}

bool Shader::Link(GLuint vertex, GLuint geometry, GLuint fragment)
{
    // Check if any of the shaders have invalid values.
    //  The geometry shader is allowed to have an invalid value ('255')
    //  in case it will purposely not be used by the shader program.
    if (vertex == 255 || fragment == 255) // This is code for "Invalid Shader handle".
    {
        printf("[Shader] Linking failed. Shaders have invalid values.\n");
        glDeleteShader(vertex);
        glDeleteShader(geometry);
        glDeleteShader(fragment);
        return false;
    }

    _program = glCreateProgram();
    glAttachShader(_program, vertex);
    glAttachShader(_program, geometry);
    glAttachShader(_program, fragment);
    glLinkProgram(_program);

    // Check for errors
    GLint link_status = 0; // Reuse the old 'compile_status' for linking status
    glGetProgramiv(_program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        GLchar program_info[1024];
        GLsizei info_length;
        glGetProgramInfoLog(_program, sizeof(program_info), &info_length, program_info);
        printf("[Shader] Program Linking error detected. Info:\n%s", program_info);

        glDetachShader(_program, vertex);
        glDetachShader(_program, geometry);
        glDetachShader(_program, fragment);
        glDeleteShader(vertex);
        glDeleteShader(geometry);
        glDeleteShader(fragment);
        glDeleteProgram(_program);
        return false;
    }

    // Detach and delete shaders
    glDetachShader(_program, vertex);
    glDetachShader(_program, geometry);
    glDetachShader(_program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);

    return true;
}
