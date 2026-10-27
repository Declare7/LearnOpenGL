#include "ModelLoadedSection.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

ModelLoadedSection::ModelLoadedSection(const std::string &type)
    : SectionBase(type, true)
{
    m_camera = std::make_unique<lngl::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

void ModelLoadedSection::prepare()
{
    m_shader = loadShader();
    m_shader->use();
    m_shader->setUniform("directionLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    m_shader->setUniform("directionLight.ambient", glm::vec3(0.2, 0.2, 0.2));
    m_shader->setUniform("directionLight.diffuse", glm::vec3(0.5, 0.5, 0.5));
    m_shader->setUniform("directionLight.specular", glm::vec3(1.0, 1.0, 1.0));

    std::string path = SOURCE_PATH;
    path += "/resource/model/backpack/backpack.obj";
    m_model = std::make_unique<lngl::Model>(path);
}

void ModelLoadedSection::render()
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    m_shader->use();
    m_shader->setUniform("viewPos", m_camera->getPosition());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4));
    m_shader->setUniform("model", model);

    glm::mat4 projection = glm::perspective(glm::radians(m_camera->getZoom()), 800.0f/600.0f, 0.1f, 100.0f);
    m_shader->setUniform("projection", projection);
    m_shader->setUniform("view", m_camera->getViewMetrix());

    m_model->draw(m_shader);
}

void ModelLoadedSection::processInput(GLFWwindow *window)
{
    float deltaTime = glfwGetTime() - m_lastTime;
    m_lastTime = glfwGetTime();

    if(glfwGetKey(window, GLFW_KEY_W))
    {
        m_camera->onMoved(lngl::Camera::MOVE_FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S))
    {
        m_camera->onMoved(lngl::Camera::MOVE_BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A))
    {
        m_camera->onMoved(lngl::Camera::MOVE_LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D))
    {
        m_camera->onMoved(lngl::Camera::MOVE_RIGHT, deltaTime);
    }
}

void ModelLoadedSection::processMouse(double msX, double msY)
{
    double xOffset = msX - m_lastMouseX;
    double yOffset = msY - m_lastMouseY;
    m_lastMouseX = msX;
    m_lastMouseY = msY;

    m_camera->onScrolled(xOffset, -yOffset);
}

void ModelLoadedSection::processWheel(double xOffset, double yOffset)
{
    m_camera->onZoomed(yOffset);
}
