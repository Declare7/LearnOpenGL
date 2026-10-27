#include "Model.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

lngl::Model::Model(const std::string &path)
{
    loadModel(path);
}

void lngl::Model::draw(const std::unique_ptr<ShaderProgram> &shader)
{
    for(auto &mesh : m_meshes)
    {
        mesh.draw(shader);
    }
}

void lngl::Model::loadModel(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout<< "ERROR::ASSIMP::"<< importer.GetErrorString()<< std::endl;
        return;
    }
    m_directory = path.substr(0, path.find_last_of("/"));

    processNode(scene->mRootNode, scene);
}

void lngl::Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i= 0; i< node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i= 0; i< node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

lngl::Mesh lngl::Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i= 0; i< mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if(mesh->mTextureCoords[0])
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(std::move(vertex));
    }

    for(unsigned int i= 0; i< mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j= 0; j< face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<lngl::Texture> lngl::Model::loadMaterialTextures(aiMaterial *material, int textureType, const std::string &typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i= 0; i< material->GetTextureCount((aiTextureType)textureType); ++i)
    {
        aiString str;
        material->GetTexture(aiTextureType(textureType), i, &str);
        bool skip = false;
        for(unsigned int j = 0; j< m_hasLoadedTextures.size(); ++j)
        {
            if(std::strcmp(str.C_Str(), m_hasLoadedTextures.at(j).path.c_str()) == 0)
            {
                textures.push_back(m_hasLoadedTextures.at(j));
                skip = true;
                break;
            }
        }

        if(!skip)
        {
            Texture text;
            text.id = textureFromFile(str.C_Str());
            text.type = typeName;
            text.path = str.C_Str();
            textures.push_back(text);
            m_hasLoadedTextures.push_back(text);
        }
    }

    return textures;
}

unsigned int lngl::Model::textureFromFile(const std::string &fileName)
{
    std::string filePath = m_directory + "/" + fileName;

    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if(data)
    {
        GLenum format = GL_RGB;
        if(channels == 1)
            format = GL_RED;
        else if(channels == 3)
            format = GL_RGB;
        else if(channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout<< "Texture failed to load at path: "<< filePath<< std::endl;

    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}
