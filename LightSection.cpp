#include "LightSection.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "3rd/stb/stb_image.h"
#include <format>

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

LightSection::LightSection(const std::string &type, bool supportInput) : SectionBase(type, supportInput)
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

    m_camera->onScrolled(xOffset, -yOffset);
}

void LightSection::processWheel(double xOffset, double yOffset)
{
    m_camera->onZoomed(yOffset);
}

void LightSection::prepare()
{
    glEnable(GL_DEPTH_TEST);
    if(m_type == "lightChanged")
    {
        m_shader = loadShader("material");
    }
    else
    {
        m_shader = loadShader(m_type);
    }

    m_lightShader = loadShader("lightSource");
    m_lightShader->use();
    m_lightShader->setUniform("lightColor", 1.0f, 1.0f, 1.0f);

    if(m_type == "texture" || m_type == "lightDirection" || m_type == "lightPoint" || m_type == "lightFlash" || m_type == "multiLight")
    {
        prepareTexture();
    }
    else
    {
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

        //light
        glGenVertexArrays(1, &m_lightVAO);
        glBindVertexArray(m_lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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
    }
}

void LightSection::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE);

    m_shader->use();
    m_shader->setUniform("viewPos", m_camera->getPosition().x, m_camera->getPosition().y, m_camera->getPosition().z);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

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

    if(m_type == "lightDirection")
    {
        renderDirection();
    }
    else if(m_type == "lightPoint")
    {
        renderPoint();
    }
    else if(m_type == "lightFlash")
    {
        renderLightFlash();
    }
    else if(m_type == "multiLight")
    {
        renderMultiLight();
    }
    else
    {
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
}

void LightSection::prepareTexture()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    //diffuse texture
    unsigned int diffuseTexture;
    glGenTextures(1, &diffuseTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width = 0;
    int height = 0;
    int channels = 0;
    std::string imgPath = SOURCE_PATH;
    imgPath += "/resource/image/container2.png";
    unsigned char *data = stbi_load(imgPath.c_str(), &width, &height, &channels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    //specular texture
    unsigned int specularTexture;
    glGenTextures(1, &specularTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    imgPath = SOURCE_PATH;
    imgPath += "/resource/image/container2_specular.png";
    data = stbi_load(imgPath.c_str(), &width, &height, &channels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    //light
    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_shader->use();
    m_shader->setUniform("material.diffuse", 0);
    m_shader->setUniform("material.specular", 1);
    m_shader->setUniform("material.shininess", 32.0f);

    if(m_type == "multiLight")
    {
        m_shader->setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
        m_shader->setUniform("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        m_shader->setUniform("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        m_shader->setUniform("dirLight.specular", 1.0f, 1.0f, 1.0f);

        for(int i= 0; i< 4; ++i)
        {
            m_shader->setUniform(std::format("pointLights[{}].position", i), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);

            m_shader->setUniform(std::format("pointLights[{}].constant", i), 1.0f);
            m_shader->setUniform(std::format("pointLights[{}].linear", i), 0.09f);
            m_shader->setUniform(std::format("pointLights[{}].quadratic", i), 0.032f);

            m_shader->setUniform(std::format("pointLights[{}].ambient", i), 0.2f, 0.2f, 0.2f);
            m_shader->setUniform(std::format("pointLights[{}].diffuse", i), 0.5f, 0.5f, 0.5f);
            m_shader->setUniform(std::format("pointLights[{}].specular", i), 1.0f, 1.0f, 1.0f);
        }

        m_shader->setUniform("spotLight.cutOffCos", glm::cos(glm::radians(8.5f)));
        m_shader->setUniform("spotLight.outerCutOffCos", glm::cos(glm::radians(12.5f)));

        m_shader->setUniform("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        m_shader->setUniform("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
        m_shader->setUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);

        m_shader->setUniform("spotLight.constant", 1.0f);
        m_shader->setUniform("spotLight.linear", 0.09f);
        m_shader->setUniform("spotLight.quadratic", 0.032f);
    }
    else
    {
        m_shader->setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
        m_shader->setUniform("light.diffuse", 0.5f, 0.5f, 0.5f);
        m_shader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);

        if(m_type == "lightDirection")
        {
            m_shader->setUniform("light.direction", -0.2f, -1.0f, -0.3f);
        }
        else if(m_type == "lightPoint" || m_type == "lightFlash")
        {
            m_shader->setUniform("light.constant", 1.0f);
            m_shader->setUniform("light.linear", 0.09f);
            m_shader->setUniform("light.quadratic", 0.032f);
        }
        else
        {
            m_shader->setUniform("light.position", 1.2f, 0.5f, 2.0f);
        }
    }
}

void LightSection::renderDirection()
{
    glm::mat4 view = glm::mat4(1.0);
    view = m_camera->getViewMetrix();
    m_shader->setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(m_camera->getZoom()), 800.0f/600, 0.1f, 100.0f);
    m_shader->setUniform("projection", projection);

    for(int i= 0; i< 10; ++i)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_shader->setUniform("model", model);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void LightSection::renderPoint()
{
    glm::mat4 view = glm::mat4(1.0);
    view = m_camera->getViewMetrix();
    m_shader->setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(m_camera->getZoom()), 800.0f/600, 0.1f, 100.0f);
    m_shader->setUniform("projection", projection);

    for(int i= 0; i< 10; ++i)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_shader->setUniform("model", model);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //light
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    m_lightShader->use();
    m_lightShader->setUniform("lightColor", lightColor.x, lightColor.y, lightColor.z);

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3(0.2f));

    m_lightShader->setUniform("model", model);
    m_lightShader->setUniform("view", view);
    m_lightShader->setUniform("projection", projection);

    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void LightSection::renderLightFlash()
{
    m_shader->setUniform("light.position", m_camera->getPosition().x, m_camera->getPosition().y, m_camera->getPosition().z);
    m_shader->setUniform("light.direction", -m_camera->getFront().x, -m_camera->getFront().y, -m_camera->getFront().z);
    m_shader->setUniform("light.cutOffCos", glm::cos(glm::radians(8.5f)));
    m_shader->setUniform("light.outerCutOffCos", glm::cos(glm::radians(12.5f)));
    renderDirection();
}

void LightSection::renderMultiLight()
{
    m_shader->setUniform("spotLight.position", m_camera->getPosition().x, m_camera->getPosition().y, m_camera->getPosition().z);
    m_shader->setUniform("spotLight.direction", -m_camera->getFront().x, -m_camera->getFront().y, -m_camera->getFront().z);

    renderDirection();

    //light
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    m_lightShader->use();
    m_lightShader->setUniform("lightColor", lightColor.x, lightColor.y, lightColor.z);

    glm::mat4 view = glm::mat4(1.0);
    view = m_camera->getViewMetrix();
    m_lightShader->setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(m_camera->getZoom()), 800.0f/600, 0.1f, 100.0f);
    m_lightShader->setUniform("projection", projection);

    glBindVertexArray(m_lightVAO);

    for(int i= 0; i< 4; ++i)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        m_lightShader->setUniform("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
