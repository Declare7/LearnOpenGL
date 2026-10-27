#ifndef DATADEFINE_H
#define DATADEFINE_H

#include <glm/glm.hpp>

namespace lngl {

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

}//end namespace lngl


#endif // DATADEFINE_H
