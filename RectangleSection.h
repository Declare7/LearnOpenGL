#ifndef RECTANGLESECTION_H
#define RECTANGLESECTION_H

#include "SectionBase.h"

class RectangleSection : public SectionBase
{
public:
    RectangleSection(const std::string &type="normal");

    void prepare() override;
    void render() override;
    void processInput(GLFWwindow *window) override;

protected:
    void prepareEBO();
    void prepareTexture();
    void prepareTextureUnit();

private:
    float m_mixRatio{0.5};
    float m_lastTime{0.0};
};

#endif // RECTANGLESECTION_H
