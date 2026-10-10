#ifndef LIGHTSECTION_H
#define LIGHTSECTION_H

#include "SectionBase.h"

class Camera;

class LightSection : public SectionBase
{
public:
    explicit LightSection(const std::string &type="normal");

public:
    virtual void prepare();
    virtual void render();

protected:
    std::unique_ptr<ShaderProgram> m_lightShader;
    std::unique_ptr<Camera> m_camera;
    unsigned int m_lightVAO{0};
};

#endif // LIGHTSECTION_H
