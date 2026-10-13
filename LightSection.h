#ifndef LIGHTSECTION_H
#define LIGHTSECTION_H

#include "SectionBase.h"

class Camera;

class LightSection : public SectionBase
{
public:
    explicit LightSection(const std::string &type="normal");

    void processInput(GLFWwindow *window) override;
    void processMouse(double msX, double msY) override;
    void processWheel(double xOffset, double yOffset) override;

public:
    virtual void prepare() override;
    virtual void render() override;

protected:
    std::unique_ptr<ShaderProgram> m_lightShader;
    std::unique_ptr<Camera> m_camera;
    unsigned int m_lightVAO{0};
    double m_lastMouseX{400.0};
    double m_lastMouseY{300.0};
    float m_lastTime{0.0};
    glm::vec3 m_lightPos{1.2f, 0.5f, 2.0f};
};

#endif // LIGHTSECTION_H
