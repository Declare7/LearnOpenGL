#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <glm/glm.hpp>

namespace lngl {

class ShaderProgram
{
public:
    ShaderProgram(const std::string &path, const std::string &name);

    void use();

    void setUniform(const std::string &name, unsigned int value) const;

    void setUniform(const std::string &name, int value) const;

    void setUniform(const std::string &name, float value) const;

    void setUniform(const std::string &name, const glm::mat4 &value) const;

    void setUniform(const std::string &name, const glm::vec3 &value) const;

    void setUniform(const std::string &name, const glm::vec4 &value) const;

private:
    void initialize(const std::string &path, const std::string &name);
    std::string readShader(const std::string &name, const std::string &suffix);

private:
    unsigned int m_shaderProgram{0};
};

}//end namespace lngl

#endif // SHADERPROGRAM_H
