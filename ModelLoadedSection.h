#ifndef MODELLOADEDSECTION_H
#define MODELLOADEDSECTION_H

#include "SectionBase.h"
#include "Model.h"

namespace lngl {
class Camera;
}

class ModelLoadedSection : public SectionBase
{
public:
    explicit ModelLoadedSection(const std::string &type="normal");

    void prepare() override;
    void render() override;

    void processInput(GLFWwindow *window) override;
    void processMouse(double msX, double msY) override;
    void processWheel(double xOffset, double yOffset) override;

private:
    std::unique_ptr<lngl::Model> m_model;
    std::unique_ptr<lngl::Camera> m_camera;
    double m_lastMouseX{400.0};
    double m_lastMouseY{300.0};
    float m_lastTime{0.0};
};

#endif // MODELLOADEDSECTION_H
