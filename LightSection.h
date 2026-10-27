#ifndef LIGHTSECTION_H
#define LIGHTSECTION_H

#include "SectionBase.h"

namespace lngl {
class Camera;
}

class LightSection : public SectionBase
{
public:
    explicit LightSection(const std::string &type="normal", bool supportInput=true);

    void processInput(GLFWwindow *window) override;
    void processMouse(double msX, double msY) override;
    void processWheel(double xOffset, double yOffset) override;

    virtual void prepare() override;
    virtual void render() override;

private:
    void prepareTexture();
    void renderDirection();
    void renderPoint();
    void renderLightFlash();
    void renderMultiLight();

protected:
    std::unique_ptr<lngl::Camera> m_camera;
    double m_lastMouseX{400.0};
    double m_lastMouseY{300.0};
    float m_lastTime{0.0};

private:
    std::unique_ptr<lngl::ShaderProgram> m_lightShader;
    glm::vec3 m_lightPos{1.2f, 0.5f, 2.0f};
    unsigned int m_lightVAO{0};
};

#endif // LIGHTSECTION_H
