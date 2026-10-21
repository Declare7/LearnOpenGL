#ifndef DATADEFINE_H
#define DATADEFINE_H

#include <glm/glm.hpp>

namespace lngl {

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
};

}//end namespace lngl


#endif // DATADEFINE_H
