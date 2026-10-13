#include "LightSection.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

LightSection::LightSection(const std::string &type) : SectionBase(type)
{
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

void LightSection::processInput(GLFWwindow *window)
{
    float deltaTime = glfwGetTime() - m_lastTime;
    m_lastTime = glfwGetTime();

    if(glfwGetKey(window, GLFW_KEY_W))
    {
        m_camera->onMoved(Camera::MOVE_FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S))
    {
        m_camera->onMoved(Camera::MOVE_BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A))
    {
        m_camera->onMoved(Camera::MOVE_LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D))
    {
        m_camera->onMoved(Camera::MOVE_RIGHT, deltaTime);
    }
}

void LightSection::processMouse(double msX, double msY)
{
    double xOffset = msX - m_lastMouseX;
    double yOffset = msY - m_lastMouseY;
    m_lastMouseX = msX;
    m_lastMouseY = msY;

    m_camera->onScrolled(xOffset, yOffset);
}

void LightSection::processWheel(double xOffset, double yOffset)
{
    m_camera->onZoomed(yOffset);
}

void LightSection::prepare()
{
    if(m_type == "lightChanged")
    {
        m_shader = loadShader("material");
    }
    else
    {
        m_shader = loadShader(m_type);
    }

    m_lightShader = loadShader("lightSource");

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //light
    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_shader->use();
    if(m_type == "material" || m_type == "lightChanged")
    {
        m_shader->setUniform("material.ambient", 0.135f, 0.2225f, 0.1575f);
        m_shader->setUniform("material.diffuse", 0.54f, 0.89f, 0.63f);
        m_shader->setUniform("material.specular", 0.316228f, 0.316228f, 0.316228f);
        m_shader->setUniform("material.shininess", 25.6f);

        m_shader->setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
        m_shader->setUniform("light.diffuse", 0.5f, 0.5f, 0.5f);
        m_shader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        m_shader->setUniform("light.position", 1.2f, 0.5f, 2.0f);
    }
    else
    {
        m_shader->setUniform("lightPos", m_lightPos.x, m_lightPos.y, m_lightPos.z);
        m_shader->setUniform("lightColor", 1.0f, 1.0f, 1.0f);
        m_shader->setUniform("objectColor", 1.0f, 0.5f, 0.31f);
    }

    m_lightShader->use();
    m_lightShader->setUniform("lightColor", 1.0f, 1.0f, 1.0f);
}

void LightSection::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE);

    m_shader->use();
    m_shader->setUniform("viewPos", m_camera->getPosition().x, m_camera->getPosition().y, m_camera->getPosition().z);
    glm::vec3 lightColor;
    if(m_type == "lightChanged")
    {
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        m_shader->setUniform("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
        m_shader->setUniform("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
    }

    glm::mat4 model = glm::mat4(1.0);
    m_shader->setUniform("model", model);

    glm::mat4 view = glm::mat4(1.0);
    view = m_camera->getViewMetrix();
    m_shader->setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(m_camera->getZoom()), 800.0f/600, 0.1f, 100.0f);
    m_shader->setUniform("projection", projection);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //light
    m_lightShader->use();
    m_lightShader->use();
    m_lightShader->setUniform("lightColor", lightColor.x, lightColor.y, lightColor.z);

    model = glm::mat4(1.0);
    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3(0.2f));

    m_lightShader->setUniform("model", model);
    m_lightShader->setUniform("view", view);
    m_lightShader->setUniform("projection", projection);

    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
