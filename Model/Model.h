#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Shaders/ShaderClass.h"
#include <iostream>
#include <vector>
#include "../Mesh/Mesh.h"
#include <future>
#include <mutex>
#include <memory>


class Model
{

public:
    Model();
    void StartModel(std::string const& path ,bool gamma = false) {
        loadSceneAsync(path,directory,isLoaded,modelMutex);
        //loadModel(path);
    }
    void Draw(ShaderClass& shader);
    bool isLoaded;
private:
    std::mutex modelMutex;
    // model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::string directory;
    bool gammaCorrection;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory,bool gamma);
    Material loadMaterial(aiMaterial* mat);
    void loadSceneAsync(std::string path, std::string& directory, bool& isLoaded, std::mutex& mutex);
    
};
