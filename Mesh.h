#ifndef MESH_H
#define MESH_H

#include "DataDefine.h"
#include "ShaderProgram.h"

namespace lngl {

class Mesh
{
public:
    explicit Mesh(const std::vector<Vertex> &vertices, const std::vector<int> &indices, const std::vector<Texture> &textures);

    void draw(const std::unique_ptr<ShaderProgram> &shader);

private:
    void setupMesh();

private:
    std::vector<Vertex> m_vertices;
    std::vector<int> m_indices;
    std::vector<Texture> m_textures;

    unsigned int m_VAO{0};
    unsigned int m_VBO{0};
    unsigned int m_EBO{0};
};

}//end namespace lngl


#endif // MESH_H
