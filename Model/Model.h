#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Shaders/ShaderClass.h"
#include <iostream>
#include <vector>
#include "../Mesh/Mesh.h"


struct Material {
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Ambient;
    float Shininess;
};
class Model
{

public:
    Model(std::string const& path, bool gamma = false)
    {
        loadModel(path);
    }
    void Draw(ShaderClass& shader);
private:
    // model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory,bool gamma);
    Material loadMaterial(aiMaterial* mat);
};
