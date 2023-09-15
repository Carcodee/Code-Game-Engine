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
    Model(const Model& other);

    void StartModel(std::string const& path ,bool isPBR,bool gamma = false) {
       myFuture = loadSceneAsync(path,directory,isLoaded,modelMutex);
       this->isPBR=isPBR;
       this->path = path;
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
    std::string path;
    bool gammaCorrection;
    bool isPBR;
    std::future<const aiScene*> myFuture;
    Assimp::Importer import;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    void LoadPBRTextures(std::string typeName,std::vector<Texture>& texture);
    unsigned int TextureFromFile(const char* path, const std::string& directory,bool gamma);
    Material loadMaterial(aiMaterial* mat);
    std::future<const aiScene*> loadSceneAsync(std::string path, std::string& directory, bool& isLoaded, std::mutex& mutex);
    
};
