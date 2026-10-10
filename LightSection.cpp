#include "LightSection.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

LightSection::LightSection(const std::string &type) : SectionBase(type)
{
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));
}

void LightSection::prepare()
{
    m_shader = loadShader(m_type);
    m_lightShader = loadShader("lightSource");

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //light
    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LightSection::render()
{
    m_shader->use();

    glm::mat4 model = glm::mat4(1.0);
    model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.1f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.1f, 0.0f));
    m_shader->setUniform("model", model);

    glm::mat4 view = m_camera->getViewMetrix();
    m_shader->setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(m_camera->getZoom()), 800.0f/600, 0.1f, 100.0f);
    m_shader->setUniform("projection", projection);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //light
    m_lightShader->use();

    glm::mat4 model2 = glm::mat4(1.0);
    model2 = glm::translate(model2, glm::vec3(-1.2f, -1.0f, -0.5f));
    model2 = glm::scale(model2, glm::vec3(0.3f));
    model2 = glm::rotate(model2, glm::radians(-20.0f), glm::vec3(0.1f, 0.0f, 0.0f));
    model2 = glm::rotate(model2, glm::radians(30.0f), glm::vec3(0.0f, 0.1f, 0.0f));

    m_lightShader->setUniform("model", model2);
    m_lightShader->setUniform("view", view);
    m_lightShader->setUniform("projection", projection);

    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
