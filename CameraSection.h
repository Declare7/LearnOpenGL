#ifndef CAMERASECTION_H
#define CAMERASECTION_H

#include "CoordinateSection.h"
#include "glm/glm.hpp"

class CameraSection : public CoordinateSection
{
public:
    CameraSection(const std::string &type="normal");

    void prepare() override;
    void render() override;
    void processInput(GLFWwindow *window) override;
    void processMouse(double msX, double msY) override;
    void processWheel(double xOffset, double yOffset) override;

protected:
    void renderAround();

    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    float m_lastTime{0.0};
    double m_lastMouseX{400.0};
    double m_lastMouseY{300.0};
    float m_pitch{0.0};
    float m_yaw{0.0};

    float m_fov{45.0};
};

#endif // CAMERASECTION_H
