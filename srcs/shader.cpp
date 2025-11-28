#include <Renderer.hpp>
#include <iostream>

std::string readShaderFile(std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open shader file " + path);
    std::stringstream buff;
    buff << file.rdbuf();
    return buff.str();
}

GLuint compileShader(GLenum type, std::string& path)
{
    std::string code = readShaderFile(path);
    const char* src = code.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Vérifier compilation
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error in shader compilation: " << infoLog << std::endl;
    }
    return shader;
}

GLuint createShaderProgram(std::string& vertexSrc, std::string& fragmentSrc)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Vérifier le link
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Error in the linking program: " << infoLog << std::endl;
    }

    // Supprimer shaders après link
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}