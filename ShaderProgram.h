#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
    ShaderProgram(const std::string &path, const std::string &name);

    void use();

    void setUniform(const std::string &name, int value);

    void setUniform(const std::string &name, const glm::mat4 &value);

    void setUniform(const std::string &name, float v1, float v2, float v3);

    void setUniform(const std::string &name, float v1, float v2, float v3, float v4);

private:
    void initialize(const std::string &path, const std::string &name);
    std::string readShader(const std::string &name, const std::string &suffix);

private:
    unsigned int m_shaderProgram{0};
};

#endif // SHADERPROGRAM_H
