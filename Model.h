#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "ShaderProgram.h"
#include "Mesh.h"
#include "DataDefine.h"

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;


namespace lngl {

class Model
{
public:
    Model(const std::string &path);

    void draw(const std::unique_ptr<ShaderProgram> &shader);

private:
    void loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *material, int textureType, const std::string &typeName);
    unsigned int textureFromFile(const std::string &fileName);

private:
    std::vector<Mesh> m_meshes;
    std::vector<Texture> m_hasLoadedTextures;
    std::string m_directory{""};
};

}//end of lngl

#endif // MODEL_H
