#include "SectionBase.h"

#include <fstream>
#include <typeinfo>

#ifdef __GNUG__
#include <cxxabi.h>
#include <memory>
#endif

#include "glad/glad.h"
#include <iostream>

SectionBase::SectionBase() {}

SectionBase::~SectionBase()
{

}

void SectionBase::loadShader(const std::string &type)
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    auto vertex = readVertexShader(type);
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
    auto fragment = readFragmentShader(type);
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

std::string SectionBase::readVertexShader(const std::string &name)
{
    return readShader(name, ".vert");
}

std::string SectionBase::readFragmentShader(const std::string &name)
{
    return readShader(name, ".frag");
}

std::string SectionBase::readShader(const std::string &name, const std::string &suffix)
{
    std::string path = "shader";
    path = SOURCE_PATH;
    path = path + "/shader/" + getClasName() + "/" + name + suffix;
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

std::string SectionBase::getClasName()
{
#ifdef __GNUG__
    int status;
    std::unique_ptr<char, void(*)(void*)> demangled(
        abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, &status),
        std::free
        );
    return (status == 0) ? demangled.get() : typeid(*this).name();
#elif defined(_MSC_VER)
    std::string name = typeid(*this).name();
    // 移除 "class " 前缀
    if (name.find("class ") == 0) name = name.substr(6);
    return name;
#else
    return typeid(*this).name();
#endif
}

