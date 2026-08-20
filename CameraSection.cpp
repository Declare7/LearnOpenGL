#include "CameraSection.h"
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CameraSection::CameraSection(const std::string &type)
    : CoordinateSection(type)
{

}

void CameraSection::prepare()
{
    loadShader();
    prepare3D();

    if(m_type == "keyboard" || m_type == "mouse")
    {
        m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

void CameraSection::render()
{
    glUseProgram(m_shaderProgram);

    if(m_type == "normal")
    {
        renderAround();
    }
    else
    {
        glm::mat4 view = glm::mat4(1.0);
        view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
        unsigned int viewLocation = glGetUniformLocation(m_shaderProgram, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    }

    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(m_fov), 800.0f/600, 0.1f, 100.0f);
    unsigned int projectionLocation = glGetUniformLocation(m_shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glm::vec3 cubePosition[] = {
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(2.0, 4.4, -12.0),
        glm::vec3(-3.3, -1.5, -3.0),
        glm::vec3(1.0, -3.0, -5.0),
        glm::vec3(-3.0, 3.7, -8.0),
        glm::vec3(-2.0, -4.1, -15.0),
        glm::vec3(2.7, -3.3, -6.0),
        glm::vec3(-1.8, -4.1, -6.6),
        glm::vec3(3.7, -1.9, -4.5),
        glm::vec3(2.0, 1.3, -4.0)
    };

    for(int i= 0; i< 10; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePosition[i]);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * (i+1)), glm::vec3(1.0f, 0.5f, 0.3f));
        unsigned int modelLocation = glGetUniformLocation(m_shaderProgram, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void CameraSection::processInput(GLFWwindow *window)
{
    if(m_type != "keyboard" && m_type != "mouse")
    {
        return;
    }

    float deltaTime = glfwGetTime() - m_lastTime;
    m_lastTime = glfwGetTime();

    float cameraSpeed = 2.5f * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_W))
    {
        m_cameraPos += cameraSpeed * m_cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_S))
    {
        m_cameraPos -= cameraSpeed * m_cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_A))
    {
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D))
    {
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    }
}

void CameraSection::processMouse(double msX, double msY)
{
    if(m_type != "mouse")
    {
        return;
    }

    static bool fir = true;
    if(fir)
    {
        m_lastMouseX = msX;
        m_lastMouseY = msY;
        fir = false;
    }

    double xOffset = msX - m_lastMouseX;
    double yOffset = msY - m_lastMouseY;
    m_lastMouseX = msX;
    m_lastMouseY = msY;

    float sensitivity = 0.05;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;
    if(m_pitch> 89.0f)
        m_pitch = 89.0f;
    else if(m_pitch< -89.0f)
        m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    front.y = sin(glm::radians(m_pitch));
    front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    m_cameraFront = glm::normalize(front);
}

void CameraSection::processWheel(double xOffset, double yOffset)
{
    if(m_type != "mouse")
    {
        return;
    }

    if(m_fov>= 1.0f && m_fov<= 60.0f)
    {
        m_fov -= yOffset;
    }
    else if(m_fov< 1.0f)
    {
        m_fov = 1.0f;
    }
    else if(m_fov> 60.0f)
    {
        m_fov = 60.0f;
    }
}

void CameraSection::renderAround()
{
    glm::mat4 view = glm::mat4(1.0);
    float radius = 10.0f;
    float cameraX = sin(glfwGetTime()) * radius;
    float cameraZ = cos(glfwGetTime()) * radius;
    view = glm::lookAt(glm::vec3(cameraX, 0.0f, cameraZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int viewLocation = glGetUniformLocation(m_shaderProgram, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
}
