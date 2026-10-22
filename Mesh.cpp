#include "Mesh.h"
#include <glad/glad.h>

lngl::Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<int> &indices, const std::vector<Texture> &textures)
    : m_vertices(vertices),
    m_indices(indices),
    m_textures(textures)
{
    setupMesh();
}

void lngl::Mesh::draw(const std::unique_ptr<ShaderProgram> &shader)
{
    unsigned int diffuseNum = 0;
    unsigned int specularNum = 0;
    for(unsigned int i= 0; i< m_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);

        std::string type = m_textures[i].type;
        std::string num = "";
        if(type == "texture_diffuse")
        {
            num = std::to_string(diffuseNum++);
        }
        else if(type == "texture_specular")
        {
            num = std::to_string(specularNum++);
        }

        std::string name = "material.";
        name += type + num;
        shader->setUniform(name, i);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void lngl::Mesh::setupMesh()
{
    //VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    //VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &(m_vertices[0]), GL_STATIC_DRAW);

    //EBO
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &(m_indices[0]), GL_STATIC_DRAW);

    //顶点
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    //顶点法线
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    //纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
