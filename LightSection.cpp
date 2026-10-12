#include "LightSection.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

LightSection::LightSection(const std::string &type) : SectionBase(type)
{
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));
}

void LightSection::processMouse(double msX, double msY)
{
    double xOffset = msX - m_lastMouseX;
    double yOffset = msY - m_lastMouseY;
    m_lastMouseX = msX;
    m_lastMouseY = msY;

    m_camera->onScrolled(xOffset, yOffset);
}

void LightSection::prepare()
{
    m_shader = loadShader(m_type);
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
    m_shader->setUniform("objectColor", 1.0f, 0.5f, 0.31f);
    m_shader->setUniform("lightColor", 1.0f, 1.0f, 1.0f);
}

void LightSection::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_MULTISAMPLE);
    m_shader->use();

    glm::vec3 lightPos(1.5f, 0.5f, 1.0f);
    m_shader->setUniform("lightPos", lightPos.x, lightPos.y, lightPos.z);
    m_shader->setUniform("viewPos", m_camera->getPosition().x, m_camera->getPosition().y, m_camera->getPosition().z);

    glm::mat4 model = glm::mat4(1.0);
    model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.1f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.1f, 0.0f));
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

    model = glm::mat4(1.0);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));

    m_lightShader->setUniform("model", model);
    m_lightShader->setUniform("view", view);
    m_lightShader->setUniform("projection", projection);

    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
