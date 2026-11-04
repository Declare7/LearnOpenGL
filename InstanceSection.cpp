#include "InstanceSection.h"
#include "glad/glad.h"
#include "ShaderProgram.h"
#include <format>

InstanceSection::InstanceSection(const std::string &type) : SectionBase(type)
{

}

void InstanceSection::prepare()
{
    m_shader = loadShader();

    float vertices[] = {
        // 位置          // 颜色
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_shader->use();
    float offsetX = 0.f;
    float offsetY = 0.f;
    int count = 0;
    for(int i= -10; i< 10; i+=2)
    {
        offsetX = (float)i/ 10 + 0.1;
        for(int j= -10; j< 10; j+=2)
        {
            offsetY = (float)j/ 10 + 0.1;
            m_shader->setUniform(std::format("offsets[{}]", count), glm::vec2(offsetX, offsetY));
            count++;
        }
    }
}

void InstanceSection::render()
{
    m_shader->use();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
}
