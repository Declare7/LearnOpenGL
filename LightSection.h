#ifndef LIGHTSECTION_H
#define LIGHTSECTION_H

#include "SectionBase.h"

class Camera;

class LightSection : public SectionBase
{
public:
    explicit LightSection(const std::string &type="normal");

    virtual void processMouse(double msX, double msY) override;

public:
    virtual void prepare() override;
    virtual void render() override;

protected:
    std::unique_ptr<ShaderProgram> m_lightShader;
    std::unique_ptr<Camera> m_camera;
    unsigned int m_lightVAO{0};
    double m_lastMouseX{400.0};
    double m_lastMouseY{300.0};
};

#endif // LIGHTSECTION_H
