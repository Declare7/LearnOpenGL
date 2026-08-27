#include "TransformSection.h"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

TransformSection::TransformSection(const std::string &type)
    : RectangleSection(type)
{

}

void TransformSection::prepare()
{
    loadShader(m_type);

    prepareTextureUnit();
}

void TransformSection::render()
{
    m_program->use();
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    m_program->setUniform("trans", trans);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
