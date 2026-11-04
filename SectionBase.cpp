#include "SectionBase.h"
#include <typeinfo>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#ifdef __GNUG__
#include <cxxabi.h>
#include <memory>
#endif

#include "ShaderProgram.h"
#include "Camera.h"

SectionBase::SectionBase(const std::string &type, bool supportInput)
    : m_type(type),
    m_supportInput(supportInput)
{
    m_camera = std::make_unique<lngl::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

SectionBase::~SectionBase()
{

}

bool SectionBase::supportInput()
{
    return m_supportInput;
}

void SectionBase::processInput(GLFWwindow *window)
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

void SectionBase::processMouse(double msX, double msY)
{
    double xOffset = msX - m_lastMouseX;
    double yOffset = msY - m_lastMouseY;
    m_lastMouseX = msX;
    m_lastMouseY = msY;

    m_camera->onScrolled(xOffset, -yOffset);
}

void SectionBase::processWheel(double xOffset, double yOffset)
{
    m_camera->onZoomed(yOffset);
}

std::unique_ptr<lngl::ShaderProgram> SectionBase::loadShader(const std::string &type)
{
    std::string path = "shader";
    path = SOURCE_PATH;
    path = path + "/shader/" + getClassName() + "/";

    return std::make_unique<lngl::ShaderProgram>(path, type);
}

std::string SectionBase::getClassName()
{
#ifdef __GNUG__
    int status;
    std::unique_ptr<char, void(*)(void*)> demangled(
        abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, &status),
        std::free
        );
    return (status == 0) ? demangled.get() : typeid(*this).name();
#elif defined(_MSC_VER)
    std::string name = typeid(*this).name();
    // 移除 "class " 前缀
    if (name.find("class ") == 0) name = name.substr(6);
    return name;
#else
    return typeid(*this).name();
#endif
}

