#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram(const std::string &path, const std::string &name)
{
    initialize(path, name);
}

void ShaderProgram::useProgram()
{
    glUseProgram(m_shaderProgram);
}

void ShaderProgram::setUniform(const std::string &name, int value)
{
    auto location = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat4 &value)
{
    auto location = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string &name, float v1, float v2, float v3, float v4)
{
    auto location = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniform4f(location, v1, v2, v3, v4);
}

void ShaderProgram::initialize(const std::string &path, const std::string &name)
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    auto vertex = readShader(path + "/"+ name, ".vert");
    const char *vertexPtr = vertex.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexPtr, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    auto fragment = readShader(path + "/"+ name, ".frag");
    const char *fragmentPtr = fragment.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentPtr, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    // check for linking errors
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string ShaderProgram::readShader(const std::string &name, const std::string &suffix)
{
    std::string path = name + suffix;
    std::fstream fs(path, std::ios::in | std::ios::binary | std::ios::ate);
    if(!fs.is_open())
    {
        return "";
    }

    size_t fileSize = fs.tellg();
    std::string content(fileSize, '\0');
    fs.seekg(0);
    fs.read(&content[0], fileSize);
    fs.close();

    return content;
}
