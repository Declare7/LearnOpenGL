#include "CoordinateSection.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "3rd/stb/stb_image.h"

CoordinateSection::CoordinateSection(const std::string &type)
    : RectangleSection(type)
{

}

void CoordinateSection::prepare()
{
    loadShader(m_type);
    if(m_type == "3D")
    {
        prepare3D();
    }
    else
    {
        prepareTextureUnit();

        m_program->use();
        //model mat
        glm::mat4 model = glm::mat4(1.0);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

        m_program->setUniform("model", model);

        //view mat
        glm::mat4 view = glm::mat4(1.0);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        m_program->setUniform("view", view);

        //projection mat
        glm::mat4 projection = glm::mat4(1.0);
        projection = glm::perspective(glm::radians(45.0f), 800.0f/600, 0.1f, 100.0f);

        m_program->setUniform("projection", projection);
    }
}

void CoordinateSection::render()
{
    if(m_type == "3D")
    {
        render3D();
    }
    else
    {
        m_program->use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void CoordinateSection::prepare3D()
{
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture
    unsigned int texture0;
    glGenTextures(1, &texture0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width = 0;
    int height = 0;
    int channels = 0;
    std::string imgPath = SOURCE_PATH;
    imgPath += "/resource/image/container.jpg";
    unsigned char* data = stbi_load(imgPath.c_str(), &width, &height, &channels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    imgPath = SOURCE_PATH;
    imgPath += "/resource/image/awesomeface.png";
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(imgPath.c_str(), &width, &height, &channels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    m_program->use();
    m_program->setUniform("texture0", 0);
    m_program->setUniform("texture1", 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CoordinateSection::render3D()
{
    m_program->use();

    glm::mat4 view = glm::mat4(1.0);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -7.0f));
    m_program->setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(45.0f), 800.0f/600, 0.1f, 100.0f);
    m_program->setUniform("projection", projection);

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
        m_program->setUniform("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
