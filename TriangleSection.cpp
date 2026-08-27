#include "TriangleSection.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>


TriangleSection::TriangleSection(const std::string &type) : SectionBase(type)
{

}

void TriangleSection::prepare()
{
    loadShader(m_type);
    if(m_type == "colors")
    {
        prepareColors();
    }
    else
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
        };

        glGenVertexArrays(1, &m_VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO); //绑定VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //在GPU中开辟空间，并把CPU的数据传到GPU

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //配置顶点属性 0对应顶点着色器layout(location=0)；当前VBO被记录在VAO中
        glEnableVertexAttribArray(0); //应用顶点属性 0对应顶点着色器layout(location=0)

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0); //解绑VBO

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // glBindVertexArray(0);
        // glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    }
}

void TriangleSection::render()
{
    if(m_type == "uniform")
    {
        renderUniform();
    }
    else
    {
        // draw our first triangle
        m_program->useProgram();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time
    }
}

void TriangleSection::renderUniform()
{
    m_program->useProgram();

    float timeVal = glfwGetTime();
    float greenVal = sin(timeVal)/ 2.0f + 0.5f;
    m_program->setUniform("uniColor", 0.0f, greenVal, 0.0f, 1.0f);

    // glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TriangleSection::prepareColors()
{
    float vertices[] = {
    -0.5f, -0.5, 0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &m_VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
